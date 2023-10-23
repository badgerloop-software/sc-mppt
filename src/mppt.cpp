#include "mppt.h"

/**
* Use incremental conductance to find maximum power point.
* Logic is explained in README.
* 
* Parameters:
*   mppt (MPPT*): structure with necessary tracking variables
*   pid (PID*): PID controller trying to find MPP
**/
void incrementalConductance(MPPT *mppt, PID *pid) {
    float I = mppt->current;
    float V = mppt->voltage;
    float dI = (mppt->current - mppt->prevCurrent) / CYCLE_MS;
    float dV = (mppt->voltage - mppt->prevCurrent) / CYCLE_MS;

    if (dI/dV + I/V > 0) { // left of MPP, increase voltage
        updatePID(pid, V + VOLTAGE_STEP, V);
    } else if (dI/dV + I/V < 0) { // right of MPP, decrease voltage
        updatePID(pid, V - VOLTAGE_STEP, V);
    }
}