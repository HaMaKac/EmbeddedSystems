#include <SimpleDHT.h>
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <Ultrasonic.h>
#include <virtuabotixRTC.h>
#include "IRbuttons.h"
#include "melody.h"
#include "setup.h"


// Wiring: SDA pin is connected to A4 and SCL pin to A5.
//IR remote: Y - pin 8, G - ground, R - power

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SimpleDHT11 dht11; //G - ground, N - power, D - 2
Ultrasonic ultrasonic(12, 13); //12 - trigger, 13 - echo

bool mainScreen = true;

  byte temperature;
  byte humidity;
  byte data[40] = {0};
  byte water_level;
  byte distance;
  byte alarmHour1=0;
  byte alarmHour2=0;
  byte alarmMinute1 = 0;
  byte alarmMinute2 = 0;
  virtuabotixRTC myRTC(4,5,7);
  byte A_hour=0;
  byte A_minute=0;
  bool alarmIsActive=false;
  

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

void printMainScreen()
{
  myRTC.updateTime();
  lcd.setCursor(0,0);
  lcd.print(myRTC.dayofmonth);
  lcd.print("/");
  lcd.print(myRTC.month);
  lcd.print(" ");
  if(myRTC.hours < 10)
  {
    lcd.print("0");
    lcd.print(myRTC.hours);
    lcd.print(":");
  }
  else
  {
    lcd.print(myRTC.hours);
    lcd.print(":");
  }
  if(myRTC.minutes < 10)
  {
    lcd.print("0");
    lcd.print(myRTC.minutes);
    lcd.print(":");
  }
  else
  {
    lcd.print(myRTC.minutes);
    lcd.print(":");
  }
  if(myRTC.seconds < 10)
  {
    lcd.print("0");
    lcd.print(myRTC.seconds);
  }
  else
  {
    lcd.print(myRTC.seconds);
  } 
  lcd.setCursor(2, 1);
  lcd.print("alarm: ");
  lcd.print(alarmHour1);
  lcd.print(alarmHour2); 
  lcd.print(":");
  lcd.print(alarmMinute1);
  lcd.print(alarmMinute2);
}

bool isHumidity()
{
  dht11.read(DHT_PIN, &temperature, &humidity, data);
  if((int)humidity > 75)
  {
    analogWrite(GREEN_LED, 100);
    return true;
  }
  else
  {
    return false;
  }
  return false;  
}

bool isWet()
{
  delay(10);
  water_level = analogRead(WATER_PIN);
  if (water_level > 50)
  {
    analogWrite(GREEN_LED, 20);
    return true;
  }
  else
  {
    return false;
  } 
  return false; 
}

bool isNear()
{
  distance = ultrasonic.read();
  //Serial.println(distance);
  if (distance < 10)
  {
    analogWrite(GREEN_LED, 5);
    return true;
  }
  else
  {
    return false;
  }
  return false;
}

bool checkTasks()
{
  return isHumidity() && isWet() && isNear();
}

void playMelody()
{
  while(alarmIsActive)
  {
    const int totalNotes = sizeof(notes) / sizeof(int);

    for (int i = 0; i < totalNotes; i++)
    {
      if(checkTasks()) 
      {
        alarmIsActive = false;
        analogWrite(GREEN_LED, 0);
        return;
      }
      printMainScreen();
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

void printSideScreen()
{
  analogWrite(GREEN_LED, 0);
  dht11.read(DHT_PIN, &temperature, &humidity, data);
  lcd.home();
  lcd.print((int)temperature);
  lcd.print("*C ");
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

  while(!IrReceiver.decode()) {printMainScreen();}
  if (IrReceiver.decode()) //first digit of hour
  {
    digit = translateIR(); 
    if(digit >=0 && digit <=2) alarmHour1 = digit;
    else cont = false; 
    printMainScreen();
    delay(500);
    IrReceiver.resume(); 
  } 
  while(!IrReceiver.decode() && cont) {printMainScreen();}
  if (IrReceiver.decode()) //second digit of hour
  {
    digit = translateIR(); 
    if(digit >=0 && digit <=9 && (digit <= 4 || alarmHour1 < 2) ) alarmHour2 = digit;    
    else cont = false; 
    printMainScreen();
    delay(500);
    IrReceiver.resume(); 
  } 
  while(!IrReceiver.decode() && cont) {printMainScreen();}
  if (IrReceiver.decode()) //first digit of minute
  {
    digit = translateIR(); 
    if(digit >=0 && digit <=5) alarmMinute1 = digit;  
    else cont = false; 
    printMainScreen();
    delay(500);
    IrReceiver.resume(); 
  } 
  while(!IrReceiver.decode() && cont) {printMainScreen();}
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
  } 
  alarmIsActive = true;
}

void setup()
{
  //Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  IrReceiver.begin(IR_PIN); // Start the receiver
  lcd.home();
  dht11.read(DHT_PIN, &temperature, &humidity, data);
  myRTC.setDS1302Time(50, 36, 14, 1, 25, 4, 2022);
}

void loop() {

  myRTC.updateTime();
  if(alarmIsActive && myRTC.hours == (alarmHour1*10 + alarmHour2) && myRTC.minutes == (alarmMinute1*10 + alarmMinute2))
  {
    analogWrite(GREEN_LED, 150);
    playMelody();
    alarmHour1=0;
    alarmHour2=0;
    alarmMinute1 = 0;
    alarmMinute2 = 0;
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