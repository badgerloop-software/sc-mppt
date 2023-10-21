#ifndef PID_H
#define PID_H

#include "mbed.h"

// duty cycle cannot exceed 0.8
#define DUTY_MAX 0.8

/**
* Represents a single PID controller.
**/
typedef struct PID {
    // tuneable control variables
    float Kp, Ki, Kd;

    // proportional, integral and derivative terms
    float proportional, integral, derivative;
    
    // store previous error and measurement
    float prevErr, prevIn;

    // size of timestep
    float timestep;

    // derivative damping term
    float tauD;
} PID;

void initializePID(PID *pid, float Kp, float Ki, float Kd);
float updatePID(PID *pid, float setpoint, float input);

#endif // PID_H