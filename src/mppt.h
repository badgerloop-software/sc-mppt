#ifndef MPPT_H
#define MPPT_H

#include "pid.h"
#include "const.h"

/**
* Keeps track of relevant variables necessary for MPPT.
* Voltage and current are necessary for incremental conductance tracking.
**/
typedef struct MPPT {
    float voltage;
    float prevVoltage;
    float current;
    float prevCurrent;
    float targetVolt;
    float power;
    float prevPower;
    float dutyCycle;
} MPPT;

float incrementalConductance(MPPT &mppt, PID &pid, FastPWM pwm);
#endif // MPPT_H