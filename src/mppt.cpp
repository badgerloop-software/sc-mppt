#include "mppt.h"


Ticker mpptUpdater;
volatile float targetVoltage = INIT_VOLT;

void mpptUpdate() {
    // Tracks last power
    static float oldPower = 0.0;
    static float stepSize = INIT_VOLT_STEP;
    static float oldCurr = 0.0;
    static float stepSize_C = INIT_VOLT_STEP;

    // Constant current mode. Try to match BMS provided charge current limit via gradient descent
    if (chargeMode == ChargeMode::CONST_CURR) {
        // Compute output current for feedback
        float totalInputPower = 0;
        for (int i = 0; i < NUM_ARRAYS; i++) {
            totalInputPower += arrayData[i].curPower;
        }
        float newCurr = totalInputPower / battVolt;

        // newCurr is farther from Limit than oldCurr
        // so, we should step in opposite direction
        if (abs(newCurr - packChargeCurrentLimit) < abs(oldCurr - packChargeCurrentLimit)) {
            stepSize_C *= -1.0;
        }

        // step NOW voltage in desired direction
        float targetVoltage_C = arrayData[0].voltage + stepSize_C;
        if (targetVoltage_C <= 0) targetVoltage_C = 0.01;
        setVoltOut(targetVoltage_C);

        // Get new target voltage from PID and update output
        // float targetVoltage = updateCurrentOut(outputCurr);
        // setVoltOut(targetVoltage);

        oldCurr = newCurr;
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
