/*
  Arduino Starter Kit example
 Project 13  - Touch Sensor Lamp

 This sketch is written to accompany Project 13 in the
 Arduino Starter Kit

 Parts required:
 1 Megohm resistor
 metal foil or copper mesh
 220 ohm resistor
 LED

 Software required :
 CapacitiveSensor library by Paul Badger
 http://www.arduino.cc/playground/Main/CapacitiveSensor

 Created 18 September 2012
 by Scott Fitzgerald

 http://www.arduino.cc/starterKit

 This example code is part of the public domain
 */

// import the library (must be located in the
// Arduino/libraries directory)
#include <CapacitiveSensor.h>

const int lecturas = 60;
const int pinSalida = 2;

// create an instance of the library
// pin 4 sends electrical energy
// pin 2 senses senses a change
CapacitiveSensor capSensor3 = CapacitiveSensor(pinSalida, 3);
CapacitiveSensor capSensor4 = CapacitiveSensor(pinSalida, 4);
CapacitiveSensor capSensor5 = CapacitiveSensor(pinSalida, 5);
CapacitiveSensor capSensor6 = CapacitiveSensor(pinSalida, 6);

// threshold for turning the lamp on
int threshold = 500;

// pin the LED is connected to
long antValue =0;

void setup() {
  // open a serial connection
  Serial.begin(9600);
  // set the LED pin as an output
  antValue = millis();
}

void loop() {
  // store the value reported by the sensor in a variable
  long sensorValue3 = lecturaSensor(capSensor3);
  
  long sensorValue4 = LOW; 
  if(capSensor4.capacitiveSensor(lecturas) > threshold){
    sensorValue4 = HIGH;
  }
  long sensorValue5 =  LOW;
  if(capSensor5.capacitiveSensor(lecturas) > threshold){
     sensorValue5 =HIGH;
  }
  long sensorValue6 =  LOW;
  if(capSensor6.capacitiveSensor(lecturas) > threshold){
     sensorValue6 =HIGH;
  }
  

  // print out the sensor value
  Serial.print("sensorValue3 ");
  Serial.println(sensorValue3);
  Serial.print("sensorValue4 ");
  Serial.println(sensorValue4);
  Serial.print("sensorValue5 ");
  Serial.println(sensorValue5);
  Serial.print("sensorValue6 ");
  Serial.println(sensorValue6);
  Serial.println("-----------");

  long value = millis() - 1000;
  Serial.println(value - antValue);
  antValue = value;
  
  delay(1000);
}

long lecturaSensor(CapacitiveSensor capSensor){
  long sensorValue = LOW;
  long value = capSensor.capacitiveSensor(lecturas);
  Serial.println(value);
  if(value > threshold){
    sensorValue = HIGH;
  }
  return sensorValue;
}

