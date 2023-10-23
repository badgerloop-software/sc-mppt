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
} MPPT;

void incrementalConductance(MPPT *mppt, PID *pid);
#endif // MPPT_H