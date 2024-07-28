#include <global_vars.h>
#include <Arduino.h>  // need String class

int I2C_ADD = 9;
int MAX_SPEED = 1500;
int MAX_ACCELORATION = 10000;
int SPEED = MAX_SPEED;
int ACCELORATION = MAX_ACCELORATION;
long DESTINATION = 0;

char HOST_CMD_STOP = 'e';
char RESET_MOTOR_POSITION = 'o';
char SET_SPEED = 's';
char SET_ACC = 'a';

