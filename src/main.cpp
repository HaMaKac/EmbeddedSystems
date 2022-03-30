#include "SimpleDHT.h"
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// IR remote: pin 8

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int pinDHT11 = 2;
SimpleDHT11 dht11; //G -ground, N - power,D - 2
int receiver = 8;
IRrecv irrecv(receiver); //Y - 8, G - ground, R - power
decode_results results;
bool mainScreen = true;
int number = 99;
  byte temperature;
  byte humidity;
  byte data[40] = {0};
  byte alarmHour=0;
  byte alarmMinute = 0;

int translateIR()
{
  switch(results.value)
  {
  case 16736925: return 11; //up
  case 16720605: return 14; //left
  case 16712445: return 10; //OK
  case 16761405: return 12; //right
  case 16754775: return 13; //down
  case 16730805: return 0;
  case 16738455: return 1;
  case 16750695: return 2;
  case 16756815: return 3;
  case 16724175: return 4;
  case 16718055: return 5;
  case 16743045: return 6;
  case 16716015: return 7;
  case 16726215: return 8;
  case 16734885: return 9;
  case 16728765: return -1; //*
  case 16732845: return -2; //#

  //default: 
  //  return 102;
  }
}
void printMainScreen()
{
  lcd.setCursor(2, 0);
  lcd.print(number); 
  lcd.setCursor(2, 1);
  lcd.print("alarm: ");
  lcd.print(alarmHour); 
  lcd.print(":");
  lcd.print(alarmMinute);
}
void printSideScreen()
{
  lcd.setCursor(2, 1);
  lcd.print((int)temperature);
  lcd.print("*C"); 
  lcd.setCursor(7, 1);
  lcd.print((int)humidity); 
  lcd.print("%"); 
}

void setup()
{
  lcd.init(); // initialize the lcd 
  lcd.backlight();
  Serial.begin(9600);
  irrecv.enableIRIn();
  lcd.home();
}
void inputAlarm()
{
  byte digit=1;
  alarmHour = 0;
  alarmMinute = 0;
  //irrecv.resume();
  if (irrecv.decode(&results)) //first digit of hour
  {
    digit = translateIR(); 
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
void loop()
{ 
  
  dht11.read(pinDHT11, &temperature, &humidity, data);
  
  if (irrecv.decode(&results))
  {
    number = translateIR(); 
    if(number==12) mainScreen = false;
    if(number==14) mainScreen = true;
    if(number==-1) inputAlarm();
    irrecv.resume();
  } 
  
  if(mainScreen)
  {
    printMainScreen();
  } else 
  {
    printSideScreen();
  }
  delay(1000);
  lcd.clear();
  
}