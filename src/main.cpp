#include <Arduino.h>
#include <global_vars.h>
#include <pin_map.h>
#include <Wire.h>
#include <AccelStepper.h>

const char HOST_CMD_STOP = 'e';
const char RESET_MOTOR_POSITION = 'o';
const char SET_SPEED = 's';
const char SET_ACC = 'a';
const char RUN_MOTOR = 'm';

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
  char* token;
  token = strtok(inputBuffer,"s");
  float speed = atof(token);
  SPEED = speed / 100 * MAX_SPEED;
  myStepper.setMaxSpeed(SPEED);
  //Serial.println("MaxSpeed set to:" + String(SPEED));
}

void set_motor_acceloration()
{
  char* token;
  token = strtok(inputBuffer, "a");
  float acc = atof(token);
  ACCELORATION = acc / 100 * MAX_ACCELORATION;
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
  char cmd = inputBuffer[0];
  //Serial.println(String(inputBuffer));
  switch (cmd)
  {
    case HOST_CMD_STOP:myStepper.stop();break;
    case RESET_MOTOR_POSITION:myStepper.setCurrentPosition(0);break;
    case SET_SPEED:set_motor_speed();break;
    case SET_ACC:set_motor_acceloration();break;
    case RUN_MOTOR:run_motor();break;
    default:break;
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