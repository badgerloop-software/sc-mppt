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
}

/**
* Updates the output value of the controller to target a certain
* setpoint given an input value. Because the actual control needs
* to be on discrete values, a Tustin transform is applied onto the
* Laplace transfer function in the complex domain.
*
* Parameters:
*   pid (PID*): PID controller to update
*   setpoint (float): the desired setpoint
*   input (float): the input value from the system
*
* Returns:
*   float: the loop output
**/
float updatePID(PID *pid, float setpoint, float input) {
    float error = setpoint - input;


    pid->proportional = pid->Kp * error;

    return 0;
}