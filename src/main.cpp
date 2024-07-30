#include <Arduino.h>
#include <global_vars.h>
#include <pin_map.h>
#include <Wire.h>
#include <AccelStepper.h>

AccelStepper myStepper(AccelStepper::DRIVER, Pls, Dir);
void run_motor()
{
  char* token;
  while(digitalRead(En))
  digitalWrite(En, LOW);
  token = strtok(inputBuffer, "m");
  //DESTINATION = strtol(str.c_str(),NULL,10);
  DESTINATION = atol(token);
  //while(!digitalRead(FarLimit))
  myStepper.moveTo(DESTINATION);
}
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
  int index=0;
  while (0 < Wire.available() && index<bufferSize-1) 
  {
    char x = Wire.read();
    inputBuffer[index++]=x;
    if(x=='\0')break;
  }
  inputBuffer[index]='\0';
  Serial.println("inputBuffer=" + String(inputBuffer));
  char x = inputBuffer[0];
    if(x == HOST_CMD_STOP) 
    {
      myStepper.stop();
    }
    else if(x == RESET_MOTOR_POSITION) 
    {
      myStepper.setCurrentPosition(0);
    }
    else if(x == SET_SPEED)
    {
      set_motor_speed();
    }
    else if(x== SET_ACC)
    {
      set_motor_acceloration();
    }
  else if(x == RUN_MOTOR)
  {
    run_motor();
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
  myStepper.setAcceleration(MAX_ACCELORATION);
}

void loop() {
  myStepper.run();
  // if(!myStepper.isRunning())
  // {
  //   digitalWrite(En, HIGH);
  // }
}