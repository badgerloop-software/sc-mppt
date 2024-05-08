#include "mppt.h"


Ticker mpptUpdater;
volatile float targetVoltage = INIT_VOLT;

void mpptUpdate() {
    // Tracks last power
    static float oldPower = 0.0;
    static float stepSize = INIT_VOLT_STEP;

    // Constant current mode. Try to match BMS provided charge current limit via PID loop
    if (chargeMode == ChargeMode::CONST_CURR) {
        // Compute output current for feedback
        float totalInputPower = 0;
        for (int i = 0; i < NUM_ARRAYS; i++) {
            totalInputPower += arrayData[i].curPower;
        }
        float outputCurr = totalInputPower / battVolt;
        
        // Get new target voltage from PID and update output
        float targetVoltage = updateCurrentOut(outputCurr);
        setVoltOut(targetVoltage);
        return;
    }

    // MPPT PO Mode
    // Update the desired target voltage to reality
    targetVoltage = arrayData[0].voltage;

    // Get total power from arrays
    float curPower = 0.0;
    for (int i = 0; i < NUM_ARRAYS; i++) {
        curPower += arrayData[i].curPower;
    }

    // If last step increased power, do bigger step in same direction. Else smaller step opposite direction
    if (curPower < oldPower) {
        stepSize *= -1;
    }
    
    // Update voltage target for arrays. Do not allow negative
    targetVoltage += stepSize;
    if (targetVoltage <= 0) targetVoltage = 0.01;
    setVoltOut(targetVoltage);

    // Update power for next cycle
    oldPower = curPower;
}


void initMPPT(std::chrono::microseconds updatePeriod) {
    mpptUpdater.attach(mpptUpdate, updatePeriod);
}
