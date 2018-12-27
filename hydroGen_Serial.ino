/////////////////////////////////////////////////////////////////
// hydroGen_Serial.ino
// HydroGen Senior Design Project
// Florida State University - Panama City Campus
// College of Electrical and Computer Engineering
// Project Date: Summer-Fall 2016
//
// Last Updated: 24 October 2016
// Author: Janna Blankenship
// Description: Program to check if time is between 6PM and
//   10PM based on RTC reading and if ambient light is below
//   a photoresistor reading of 100. Relay position is set 
//   accordingly. (If both time and light requirements are met,
//   relay is turned on.)
//   ** Serial version includes printouts to serial monitor 
//      for use while system is connected to a computer **
//
// Arduino Model: Arduino Pro Mini
// Connected Hardware:
//   FTDI Serial TTL-232 USB Cable
//   DS1307 Real Time Clock Breakout I2C Board (Adafruit)
//      - pins VCC, GND, A4, A5 
//   Cds Photoresistor with 100k pull-down resistor
//      - pins VCC, GND, A0
//   Tolako 5V Relay Module for Arduino (Active High)
//      - pins VCC, GND, 7
/////////////////////////////////////////////////////////////////

#include <Wire.h>
#include "RTClib.h"

#define relayPin 7 // relay attached to digital pin 7
#define photocellPin 0 // photo cell attached to analog pin 0

RTC_DS1307 rtc; // real time clock
DateTime nowInfo; // stores data about current time
int nowHour; // integer to store numerical hour value
bool relayVal = 0; // boolean value to keep track of relay on/off status
int photocellReading; // integer to store value from reading of photocell

void setup () {
  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }

  // adjust rtc to computer's time if needed
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 

  pinMode(relayPin, OUTPUT); // set pin 7 to output to send signals to relay
  digitalWrite(relayPin,0); // make sure relay is off
}

void loop () {
  nowInfo = rtc.now();
  nowHour = nowInfo.hour();

  Serial.print(nowInfo.hour(), DEC);
  Serial.print(':');
  Serial.print(nowInfo.minute(), DEC);
  Serial.println();

  if((nowHour > 17) && (nowHour < 23)) {
    Serial.println("In Specified Time Range");
    photocellReading = analogRead(photocellPin);
    Serial.print("Photocell Reading = ");
    Serial.print(photocellReading);
    Serial.println();

    if (photocellReading < 100) {
      Serial.println("Dark");
      if (relayVal == 0) {
        digitalWrite(relayPin,1);
        relayVal = 1;
      } // turn relay on if previously off
      Serial.println("Relay On");
    } // ambient light less than 10 lux
    else {
      Serial.println("Light");
      if (relayVal == 1) {
        digitalWrite(relayPin,0);
        relayVal = 0;
      } // turn relay off if previously on
      Serial.println("Relay Off");
    }
  } // greater than 5pm and less than 11pm 
  else {
    Serial.println("NOT In Specified Time Range");
    if (relayVal == 1) {
      digitalWrite(relayPin,0);
      relayVal = 0;
    } // turn relay off if previously on
    Serial.println("Relay Off");
  }
  delay(10000);
//  delay(600000); // loop repeats every 10 minutes
}
