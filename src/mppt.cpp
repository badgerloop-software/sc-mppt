#include "mppt.h"
#include "dataManagement.h"

/**
* Use incremental conductance to find maximum power point (MPP).
* Logic is explained in README.
* 
* Parameters:
*   mppt (MPPT*): structure with necessary tracking variables
*   pid (PID*): PID controller trying to find MPP
**/
// float incrementalConductance(MPPT &mppt, PID &pid, FastPWM pwm) {
//     float I = mppt.current;
//     float V = mppt.voltage;
//     float dI = (mppt.current - mppt.prevCurrent) / CYCLE_MS;
//     float dV = (mppt.voltage - mppt.prevCurrent) / CYCLE_MS;
//     float output = -1;

//     // if (dI/dV + I/V > 0) { // left of MPP, increase voltage
//     //     output = updatePID(pid, V + VOLTAGE_STEP, V);
//     // } else if (dI/dV + I/V < 0) { // right of MPP, decrease voltage
//     //     output = updatePID(pid, V - VOLTAGE_STEP, V);
//     // }

//     // return output;
// }