#include "pid.h"

/**
* Initializes an instance of a PID controller with a set of control values.
*
* Parameters:
*   pid (PID*): pointer to PID structure being initialized
*   Kp (float): proportional control term
*   Ki (float): proportional integral term
*   Kd (float): proportional derivative term
**/
void initializePID(PID *pid, float Kp, float Ki, float Kd) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->timestep = CYCLE_MS;

    // set previous values to default 0
    pid->prevErr = 0;
    pid->prevIn = 0;

    // for now, default the derivative damping term to 0
    pid->tauD = 0;
}

/**
* Updates the output value of the controller to target a certain
* setpoint given an input value. Reasoning behind equations in README.
*
* Parameters:
*   pid (PID*): PID controller to update
*   setpoint (float): the desired setpoint
*   input (float): the input value from the system
*
* Returns:
*   float: the control value out
**/
float updatePID(PID &pid, float setpoint, float input) {
    float error = setpoint - input;

    // proportional term
    pid.proportional = pid.Kp * error;

    // integral term
    pid.integral = pid.integral + 0.5f * pid.Ki * pid.timestep * (error + pid.prevErr);
    // integral will probably need anti-wind up, can be added below

    // derivative term
    pid.derivative = -(2.0f * pid.Kd * (input - pid.prevIn)	/* derivative on measurement, so the equation is negative */
                        + (2.0f * pid.tauD - pid.timestep) * pid.derivative)
                        / (2.0f * pid.tauD + pid.timestep);

    // store terms for next calculation
    pid.prevErr = error;
    pid.prevIn = input;

    float unboundedOut = pid.proportional + pid.integral + pid.derivative;
    // bind output between 0 and duty cycle max
    return (unboundedOut < 0) ? 0 : ((unboundedOut > DUTY_MAX) ? DUTY_MAX : unboundedOut);
}

/**
* Print the values stored within the PID for debugging purposes.
* 
* Parameters:
*   pid (PID*): PID controller whose data to print
**/
void printPID(PID *pid) {
    printf("Control variables:\nK_p: %f, K_i: %f, K_d: %f\nDerivative clamping term: %f\nTimestep: %f\n", pid->Kp, pid->Ki, pid->Kd, pid->tauD, pid->timestep);
    printf("Currently stored values:\nP_n: %f, I_n: %f, D_n: %f\nError: %f, Input: %f\n", pid->proportional, pid->integral, pid->derivative, pid->prevErr, pid->prevIn);
}