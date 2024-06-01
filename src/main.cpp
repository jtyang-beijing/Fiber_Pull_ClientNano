#include <Arduino.h>
#include <global_vars.h>
#include <pin_map.h>
#include <Wire.h>
#include <AccelStepper.h>

AccelStepper myStepper(AccelStepper::DRIVER, Pls, Dir);

void receiveEvent(int i) 
{
  String str = "";
  while (0 < Wire.available()) {
    char x = Wire.read();
    if(x == HOST_CMD_STOP) 
    {
      myStepper.stop();
      break;
    }
    if(x == RESET_MOTOR_POSITION) 
    {
      myStepper.setCurrentPosition(0);
      break;
    }
    str += x;
  }
  if(str != "")
  {
    while(digitalRead(En))
    digitalWrite(En, LOW);
    DESTINATION = strtol(str.c_str(),NULL,10);
    myStepper.moveTo(DESTINATION);
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
  //digitalWrite(En, HIGH);// disable motor. AccelStepper.run will handle it.
  Wire.begin(I2C_ADD);
  Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  myStepper.setMaxSpeed(MAX_SPEED);
  myStepper.setAcceleration(ACCELORATION);
}

void loop() {
  myStepper.run();
  // if(!myStepper.isRunning())
  // {
  //   digitalWrite(En, HIGH);
  // }
}