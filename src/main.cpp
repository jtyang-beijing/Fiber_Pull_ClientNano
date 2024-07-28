#include <Arduino.h>
#include <global_vars.h>
#include <pin_map.h>
#include <Wire.h>
#include <AccelStepper.h>

AccelStepper myStepper(AccelStepper::DRIVER, Pls, Dir);
void set_motor_speed()
{
  String speed = "";
  char x;
  while (0< Wire.available())
  {
    x = Wire.read();
    if (x == '\n') break;
    speed +=x;
  }
  SPEED = speed.toFloat() / 100 * MAX_SPEED;
  myStepper.setMaxSpeed(SPEED);
}

void set_motor_acceloration()
{
  String acc = "";
  char x;
  while (0< Wire.available())
  {
    x = Wire.read();
    if (x == '\n') break;
    acc +=x;
  }
  ACCELORATION = acc.toFloat() / 100 * MAX_ACCELORATION;
  myStepper.setAcceleration(ACCELORATION);
}

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
    else if(x == RESET_MOTOR_POSITION) 
    {
      myStepper.setCurrentPosition(0);
      break;
    }
    else if(x == SET_SPEED)
    {
      delay(100);
      set_motor_speed();
      break;
    }
    else if(x== SET_ACC)
    {
      delay(100);
      set_motor_acceloration();
      break;
    }
    str += x;
  }
  if(str != "")
  {
    while(digitalRead(En))
    digitalWrite(En, LOW);
    DESTINATION = strtol(str.c_str(),NULL,10);
    //while(!digitalRead(FarLimit))
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
  //Serial.begin(115200);
  myStepper.setMaxSpeed(MAX_SPEED);
  myStepper.setAcceleration(MAX_ACCELORATION);
}

void loop() {
  myStepper.run();
  // if(!myStepper.isRunning())
  // {
  //   digitalWrite(En, HIGH);
  // }
}