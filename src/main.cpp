#include "SimpleDHT.h"
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include "IRbuttons.h"

#define IR_RECEIVE_PIN 8

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// IR remote: pin 8

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int pinDHT11 = 2;
SimpleDHT11 dht11; //G -ground, N - power, D - 2
//Y - 8, G - ground, R - power
bool mainScreen = true;
int number = 99;
  byte temperature;
  byte humidity;
  byte data[40] = {0};
  byte alarmHour1=0;
  byte alarmHour2=0;
  byte alarmMinute1 = 0;
  byte alarmMinute2 = 0;

int digit=0;

void printMainScreen()
{
  lcd.setCursor(2, 0);
  lcd.print(number); 
  lcd.setCursor(2, 1);
  lcd.print("alarm: ");
  lcd.print(alarmHour1);
  lcd.print(alarmHour2); 
  lcd.print(":");
  lcd.print(alarmMinute1);
  lcd.print(alarmMinute2);
}
void printSideScreen()
{
  dht11.read(pinDHT11, &temperature, &humidity, data);
  lcd.setCursor(2, 1);
  lcd.print((int)temperature);
  lcd.print("*C"); 
  lcd.setCursor(7, 1);
  lcd.print((int)humidity); 
  lcd.print("%"); 
}

void translateIR()
{
  IrReceiver.decode();
  
  int tree = IrReceiver.decodedIRData.command;
  switch (tree)
  {
    case IR_BUTTON_5: digit = 5;
  }
  IrReceiver.resume();
}
/*
void inputAlarm()
{
  
  //irrecv.resume();
  if (IrReceiver.decode()) //first digit of hour
  {
    IrReceiver.resume();
    int command = IrReceiver.decodedIRData.command;
    if(digit >=0 && digit <=2) alarmHour += digit*10; 
    delay(500);   
    //alarmHour=11;
    //irrecv.resume();
    Serial.println(digit);
    printMainScreen();
    delay(500); 
  } 
  if (irrecv.decode(&results)) //second digit of hour
  {
    digit = translateIR(); 
    if(digit >=0 && digit <=9 && (digit <= 4 || alarmHour < 20) ) alarmHour += digit;    
    delay(500);
    //alarmHour=12;
    //irrecv.resume();
    Serial.println(digit);
    printMainScreen();
    delay(500); 
  } 
  if (irrecv.decode(&results)) //first digit of minute
  {
    digit = translateIR(); 
    if(digit >=0 && digit <=5) alarmMinute += digit*10;  
    delay(500);  
    //alarmMinute=11;
    //irrecv.resume();
    Serial.println(digit);
    printMainScreen();
    delay(500); 
  } 
  if (irrecv.decode(&results)) //second digit of minute
  {
    digit = translateIR(); 
    if(digit >=0 && digit <=9) alarmMinute += digit;    
    delay(500);
    //alarmMinute=12;
    //irrecv.resume();
    Serial.println(digit);
    printMainScreen();
    delay(500); 
  } 
}
*/

void setup()
{
  lcd.init(); // initialize the lcd 
  lcd.backlight();
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN); // Start the receiver
  lcd.home();
}

void loop() {

  //dht11.read(pinDHT11, &temperature, &humidity, data);

  if (IrReceiver.decode())
  {
    IrReceiver.resume();
    int command = IrReceiver.decodedIRData.command;
    if (command == IR_BUTTON_RIGHT) 
    { 
      lcd.clear();
      mainScreen = false;
    }
    if (command == IR_BUTTON_LEFT)
    { 
      lcd.clear();
      mainScreen = true;
    }
    if (command == IR_BUTTON_OK) 
    {
      lcd.clear();
      //inputAlarm();
      translateIR();
      Serial.println(digit);
    }
  }
  
  if(mainScreen)
  {
    //lcd.clear();
    printMainScreen();
  } else 
  {
    //lcd.clear();
    printSideScreen();
  }
  //lcd.clear();

}