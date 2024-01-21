#include "mppt.h"


Ticker mpptUpdater;


void mpptUpdate() {
    // Constant current mode
    if (chargeMode == ChargeMode::CONST_CURR) {
        // TEMPORARY: SHUTOFF CHARGING
        for (int i = 0; i < NUM_ARRAYS; i++) {
            setPWMOut(i, 0);
        }
        return;
    }

    // MPPT PO Mode
    for (int i = 0; i < NUM_ARRAYS; i++) {
        // Last step increased power, bigger step in same direction
        if (arrayData[i].curPower > arrayData[i].lastPower) {
            arrayData[i].step = 1.5 * arrayData[i].step;
        // Else lowered power, smaller step in opposite direction
        } else {
            arrayData[i].step = -0.5 * arrayData[i].step;
        }

        // Cap step size, do not allow zero 
        if (arrayData[i].step > MAX_VOLT_STEP) arrayData[i].step = MAX_VOLT_STEP;
        else if (arrayData[i].step < -MAX_VOLT_STEP) arrayData[i].step = -MAX_VOLT_STEP;
        else if (arrayData[i].step == 0) arrayData[i].step = 0.000000001;

        setPWMOut(i, arrayData[i].dutyCycle + arrayData[i].step);
    }
}


void initMPPT(std::chrono::microseconds updatePeriod) {
    mpptUpdater.attach(mpptUpdate, updatePeriod);
}
