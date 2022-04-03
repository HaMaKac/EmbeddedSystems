#include <SimpleDHT.h>
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include "virtuabotixRTC.h"
#include "IRbuttons.h"
#include "melody.h"
#include "setup.h"



// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// IR remote: pin 8

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SimpleDHT11 dht11; //G -ground, N - power, D - 2
//IR remote: Y - 8, G - ground, R - power
bool mainScreen = true;
//int number = 99;
  byte temperature;
  byte humidity;
  byte data[40] = {0};
  byte alarmHour1=0;
  byte alarmHour2=0;
  byte alarmMinute1 = 0;
  byte alarmMinute2 = 0;
  virtuabotixRTC myRTC(4,5,6); 
  byte A_hour=0;
  byte A_minute=0;
  byte AlarmIsActive=0;


int digit=0;

int translateIR()
{
  int key = IrReceiver.decodedIRData.command;
  switch(key)
  {
  case IR_BUTTON_0: return 0;
  case IR_BUTTON_1: return 1;
  case IR_BUTTON_2: return 2;
  case IR_BUTTON_3: return 3;
  case IR_BUTTON_4: return 4;
  case IR_BUTTON_5: return 5;
  case IR_BUTTON_6: return 6;
  case IR_BUTTON_7: return 7;
  case IR_BUTTON_8: return 8;
  case IR_BUTTON_9: return 9;
  }
}

bool isHumidity()
{
  dht11.read(DHT_PIN, &temperature, &humidity, data);
  if((int)humidity > 50) return true;
  else return false;
}

bool checkTasks()
{
  return isHumidity();
}

void playMelody()
{
  while(true)
  {
    const int totalNotes = sizeof(notes) / sizeof(int);

    for (int i = 0; i < totalNotes; i++)
    {
      if(checkTasks) return;
      const int currentNote = notes[i];
      float wait = durations[i];
      // Play tone if currentNote is not 0 frequency, otherwise pause (noTone)
      if (currentNote != 0)
      {
        tone(BUZZER_PIN, notes[i], wait); // tone(pin, frequency, duration)
      }
      else
      {
        noTone(BUZZER_PIN);
      }
      // delay is used to wait for tone to finish playing before moving to next loop
      delay(wait);
    }
  }
}

void printMainScreen()
{
  lcd.setCursor(0,0);
  lcd.print(myRTC.dayofmonth);
  lcd.print("/");
  lcd.print(myRTC.month);
  //lcd.print("/");
  //lcd.print(myRTC.year);
  lcd.print(" ");
  lcd.print(myRTC.hours);
  lcd.print(":");
  lcd.print(myRTC.minutes);
  lcd.print(":");
  lcd.print(myRTC.seconds);
  //lcd.setCursor(2, 0);
  //lcd.print(number); 
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
  dht11.read(DHT_PIN, &temperature, &humidity, data);
  lcd.setCursor(2, 1);
  lcd.print((int)temperature);
  lcd.print("*C"); 
  lcd.setCursor(7, 1);
  lcd.print((int)humidity); 
  lcd.print("%"); 
}

void inputAlarm()
{
  alarmHour1 = 0;
  alarmHour2 = 0;
  alarmMinute1 = 0;
  alarmMinute2 = 0;
  printMainScreen();
  delay(500);
  IrReceiver.resume();
  bool cont = true;

  while(!IrReceiver.decode()) {}
  if (IrReceiver.decode()) //first digit of hour
  {
    digit = translateIR(); 
    if(digit >=0 && digit <=2) alarmHour1 = digit;
    else cont = false; 
    printMainScreen();
    delay(500);
    IrReceiver.resume(); 
  } 
  while(!IrReceiver.decode() && cont) {}
  if (IrReceiver.decode()) //second digit of hour
  {
    digit = translateIR(); 
    if(digit >=0 && digit <=9 && (digit <= 4 || alarmHour1 < 2) ) alarmHour2 = digit;    
    else cont = false; 
    printMainScreen();
    delay(500);
    IrReceiver.resume(); 
  } 
  while(!IrReceiver.decode() && cont) {}
  if (IrReceiver.decode()) //first digit of minute
  {
    digit = translateIR(); 
    if(digit >=0 && digit <=5) alarmMinute1 = digit;  
    else cont = false; 
    printMainScreen();
    delay(500);
    IrReceiver.resume(); 
  } 
  while(!IrReceiver.decode() && cont) {}
  if (IrReceiver.decode()) //second digit of minute
  {
    digit = translateIR(); 
    if(digit >=0 && digit <=9) alarmMinute2 = digit;    
    else cont = false; 
    printMainScreen();
    delay(500);
    IrReceiver.resume(); 
  }
  if (!cont) 
  {
    lcd.setCursor(0, 0);
    //lcd.print("try again");
    //delay(2000);
  } 
}

void setup()
{
  //Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  IrReceiver.begin(IR_PIN); // Start the receiver
  lcd.home();
  dht11.read(DHT_PIN, &temperature, &humidity, data);
  myRTC.setDS1302Time(00, 7, 1, 1, 4, 4, 2022);
}

void loop() {

myRTC.updateTime();
 
  if(myRTC.hours == alarmHour1 + alarmHour2 && myRTC.minutes == alarmMinute1 + alarmMinute2)
  {
    playMelody();
  }

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
    if (command == IR_BUTTON_AST) 
    {
      lcd.clear();
      inputAlarm();
      translateIR();
    }
  }
  
  if(mainScreen)
  {
    printMainScreen();
  } else 
  {
    printSideScreen();
  }
}
