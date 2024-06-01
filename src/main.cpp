#include <Arduino.h>
#include <global_vars.h>
#include <pin_map.h>
#include <Wire.h>
#include <AccelStepper.h>

long destination;
AccelStepper myStepper(AccelStepper::DRIVER, Pls, Dir);

void receiveEvent(int i) 
{
  String str = "";
  while (0 < Wire.available()) {
    char x = Wire.read();
    if(x == HOST_CMD_STOP) 
    {
      destination = myStepper.currentPosition();
      myStepper.moveTo(destination);
      break;
    }
    str += x;
  }
  if(str != "")
  {
    destination = strtol(str.c_str(),NULL,10);
    myStepper.moveTo(destination);
  }
}

void requestEvent() {
  String str = String(myStepper.currentPosition()) + '\n';
  Wire.write(str.c_str()); // Send a response back to the master
}
void setup() {
  pinMode(En, OUTPUT);
  pinMode(Dir, OUTPUT);
  pinMode(Pls, OUTPUT);
  Wire.begin(I2C_ADD);
  Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  //Serial.println("I2C Slave Demonstration");
  myStepper.setMaxSpeed(MAX_SPEED);
  myStepper.setAcceleration(ACCELORATION);
  destination=0;
}

void loop() {
myStepper.run();
}