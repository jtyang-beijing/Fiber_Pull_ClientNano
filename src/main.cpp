#include <Arduino.h>
#include <global_vars.h>
#include <pin_map.h>
#include <Wire.h>
#include <AccelStepper.h>
void receiveEvent(int howMany);
void requestEvent();
void monitorHostCmd();
long destination;
AccelStepper myStepper(AccelStepper::DRIVER, Pls, Dir);
String str = "";
void setup() {
  pinMode(En, OUTPUT);
  pinMode(Dir, OUTPUT);
  pinMode(Pls, OUTPUT);
  Wire.begin(I2C_ADD);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  myStepper.setMaxSpeed(MAX_SPEED);
  myStepper.setAcceleration(ACCELORATION);
  destination=0;
}

void loop() {
  myStepper.run();
}

void receiveEvent(int howMany) 
{
  while (Wire.available()) 
  {
    char c = Wire.read();
    if(c == HOST_CMD_STOP) 
    {
      destination = myStepper.currentPosition();
      myStepper.moveTo(destination);
      break;
    }
    str += c;
  }
  if(str != "")
  {
    destination = strtol(str.c_str(),NULL,10);
    myStepper.moveTo(destination);
  }
}

void requestEvent() {
  str = String(myStepper.currentPosition()) + '\n';
  Wire.write(str.c_str()); // Send a response back to the master
}

