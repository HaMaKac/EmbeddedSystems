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

void setup()
{
  lcd.init(); // initialize the lcd 
  lcd.backlight(); //
  Serial.begin(9600);
  irrecv.enableIRIn(); //
  //pinMode(LED_BUILTIN, OUTPUT);
  lcd.setCursor(2, 0); // Set the cursor on the third column and first row.
  lcd.print("Hello World!"); // Print the string "Hello World!"
  lcd.home();
}

void loop()
{ 
  if (irrecv.decode(&results))
  {
    int number = translateIR(); 
    byte temperature;
  byte humidity;
  byte data[40] = {0};
  dht11.read(pinDHT11, &temperature, &humidity, data);
  //digitalWrite(LED_BUILTIN, LOW);
    irrecv.resume();
    //Serial.println(number);
    lcd.setCursor(2, 0);
    lcd.print(number); 
    lcd.setCursor(2, 1);
    lcd.print((int)temperature);
    lcd.print("*C"); 
    lcd.setCursor(7, 1);
    lcd.print((int)humidity); 
    lcd.print("%"); 
    

    
    delay(1000);
    lcd.clear();
  } 
  
  
  
  
  //Serial.print((int)temperature); Serial.print(" *C, ");
  //Serial.print((int)humidity); Serial.println(" %");


  delay(1000);
}