#include <SPI.h>          //Library for using SPI Communication 
#include <mcp2515.h>      //Library for using CAN Communication
#include <DHT.h>          //Library for using DHT sensor 

#define DHTPIN A0       
#define DHTTYPE DHT11


int myButton = 7;
int myButtonPress = 0;

int ledLight = 5;




struct can_frame canMsg; // structure where we store can msg
MCP2515 mcp2515(10);

DHT dht(DHTPIN, DHTTYPE);     //initilize object dht for class DHT with DHT pin with STM32 and DHT type as DHT11

void setup() 
{
  while (!Serial);
  Serial.begin(9600);
  
  SPI.begin();               //Begins SPI communication
  dht.begin();               //Begins to read temperature & humidity sensor value
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();

  pinMode(myButton,INPUT);
  pinMode(ledLight,OUTPUT);
}

void loop() 
{
 myButtonPress = digitalRead(myButton);

if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
{
 Serial.println("Got it");
}
 
 if(myButtonPress == HIGH)
 {
  canMsg.can_id  = 0x024;           //CAN id as 0x024
  canMsg.can_dlc = 8;               //CAN data length as 8
  canMsg.data[0] = 5;               //random data, not used at this time
  canMsg.data[1] = 2;               //random data ""
  canMsg.data[2] = 0x00;            //Reset all with 0
  canMsg.data[3] = 0x00;
  canMsg.data[4] = 0x00;
  canMsg.data[5] = 0x00;
  canMsg.data[6] = 0x00;
  canMsg.data[7] = 0x00;
  mcp2515.sendMessage(&canMsg);     //Sends the CAN message
  
  delay(1000);
 }
  int h = dht.readHumidity();       //Gets Humidity value
  int t = dht.readTemperature();    //Gets Temperature value *** currently in C. change to t*1.8+32 for F. 

  canMsg.can_id  = 0x036;           //CAN id as 0x036
  canMsg.can_dlc = 8;               //CAN data length as 8
  canMsg.data[0] = h;               //Update humidity value in [0]
  canMsg.data[1] = t;               //Update temperature value in [1]
  canMsg.data[2] = 0x00;            //Reset all with 0
  canMsg.data[3] = 0x00;
  canMsg.data[4] = 0x00;
  canMsg.data[5] = 0x00;
  canMsg.data[6] = 0x00;
  canMsg.data[7] = 0x00;
  mcp2515.sendMessage(&canMsg);     //Sends the CAN message
  delay(1000);

}
