#ifndef VARIABELS_H
#define VARIABLES_H

#include <Arduino.h>  // need String Class 
extern int I2C_ADD;
extern int MAX_SPEED;
extern int SPEED;
extern int MAX_ACCELORATION;
extern int ACCELORATION;
extern long DESTINATION;
extern const char HOST_CMD_STOP;
extern const char RESET_MOTOR_POSITION;
extern const char SET_SPEED;
extern const char SET_ACC;
extern const char RUN_MOTOR;

extern const int bufferSize;
extern char inputBuffer[];
#endif