#include <SPI.h>              //Library for using SPI Communication 
#include <mcp2515.h>          //Library for using CAN Communication
#include <LiquidCrystal.h>    //Library for using LCD display

const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8;  

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);  //Define LCD display pins RS,E,D4,D5,D6,D7

int ledSwitch = 1;

int reading = 0;

struct can_frame canMsg; 
MCP2515 mcp2515(10);                 // SPI CS Pin 10 
 
void setup() {
  pinMode(ledSwitch, INPUT);
  
  lcd.begin(16,2);                   //Sets LCD as 16x2 type
  lcd.setCursor(0,0);                //Display Welcome Message
  lcd.print("LCD CHECK");
  lcd.setCursor(0,1);
  lcd.print("LCD OK");
  delay(3000);
  lcd.clear();
  
  SPI.begin();                       //Begins SPI communication
  
  Serial.begin(9600);                //Begins Serial Communication at 9600 baudrate 
  
  mcp2515.reset();                          
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz 
  mcp2515.setNormalMode();                  //Sets CAN at normal mode
}

void loop() 
{

  if( reading == HIGH) // if high detected on pin1
  {
  canMsg.can_id  = 0x004;           //CAN id as 0x004
  canMsg.can_dlc = 8;               //CAN data length as 8
  canMsg.data[0] = 9;               //random data, not used at this time
  canMsg.data[1] = 4;               //random data ""
  canMsg.data[2] = 0x00;            //Reset all with 0
  canMsg.data[3] = 0x00;
  canMsg.data[4] = 0x00;
  canMsg.data[5] = 0x00;
  canMsg.data[6] = 0x00;
  canMsg.data[7] = 0x00;
  mcp2515.sendMessage(&canMsg);     //Sends the CAN message
  delay(1000);
  }

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) // To receive data (Poll Read)
  {
    if(canMsg.can_id  == 0x036)
    {
     int x = canMsg.data[0];         
     int y = canMsg.data[1];       
      
      lcd.setCursor(0,0);          //Display Temp & Humidity value received at 16x2 LCD
      lcd.print("Humidity : ");
      lcd.print(x);
      lcd.setCursor(0,1);
      lcd.print("Temp : ");
      lcd.print(y);
      delay(1000);
      lcd.clear();
    }
    if(canMsg.can_id == 0x024)
    {
      lcd.setCursor(0,0);
      lcd.print("Button");
      lcd.setCursor(0,1);
      lcd.print("pressed");
      delay(1000);
      lcd.clear();
    }
  }
}
