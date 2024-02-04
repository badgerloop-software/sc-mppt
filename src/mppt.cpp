#include "mppt.h"


Ticker mpptUpdater;


void mpptUpdate() {
    // Tracks last power
    static float oldPower = 0.0;
    static float stepSize = INIT_VOLT_STEP;
    static float targetVoltage = INIT_VOLT;

    // Constant current mode
    if (chargeMode == ChargeMode::CONST_CURR) {
        // TEMPORARY: SHUTOFF CHARGING
        for (int i = 0; i < NUM_ARRAYS; i++) {
            setVoltOut(i, 0);
        }
        return;
    }

    // MPPT PO Mode
    // Get total power from arrays
    float curPower = 0.0;
    for (int i = 0; i < NUM_ARRAYS; i++) {
        curPower += arrayData[i].curPower;
    }

    // If last step increased power, do bigger step in same direction. Else smaller step opposite direction
    if (curPower < oldPower) {
        stepSize *= -0.8;
    } else {
        stepSize *= 1.4;
    }

    // Make sure step size not too large, do not allow 0
    if (stepSize > MAX_VOLT_STEP) stepSize = MAX_VOLT_STEP;
    else if (stepSize < -MAX_VOLT_STEP) stepSize = -MAX_VOLT_STEP;
    else if (stepSize == 0) stepSize = 0.000000001;

    // Update voltage target for arrays
    targetVoltage += stepSize;
    for (int i = 0; i < NUM_ARRAYS; i++) {
        setVoltOut(i, targetVoltage);
    }

    // Update power for next cycle
    oldPower = curPower;
}


void initMPPT(std::chrono::microseconds updatePeriod) {
    mpptUpdater.attach(mpptUpdate, updatePeriod);
}
