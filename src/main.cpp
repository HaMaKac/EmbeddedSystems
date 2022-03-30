#include "SimpleDHT.h"
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include "IRbuttons.h"

#define IR_RECEIVE_PIN 8

void setup()
{
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN); // Start the receiver
}

void loop() {
  if (IrReceiver.decode()) 
  {
      IrReceiver.resume();
      int command = IrReceiver.decodedIRData.command;
      Serial.println(command);
      if(command == IR_BUTTON_UP) {
        IrReceiver.resume();
        Serial.println("Hello");
      }
  }
}