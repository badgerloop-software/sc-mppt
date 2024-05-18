#include "mppt.h"


Ticker mpptUpdater;
volatile float targetVoltage = INIT_VOLT;

void mpptUpdate() {
    // Tracks last power
    static float oldPower = 0.0;
    static float stepSize = INIT_VOLT_STEP;
    static bool past_BE = false;

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

    /* FEATURE 1
    // reset the targetVoltage on boostEnabled rising edge
    if (!past_BE && boostEnabled) {
        targetVoltage = 24; // (arrayData[0].voltage + 9) / 2; // start at the middle value since it is closer to MAX
    }
    past_BE = boostEnabled;
    */

    // Get total power from arrays
    float curPower = 0.0;
    for (int i = 0; i < NUM_ARRAYS; i++) {
        curPower += arrayData[i].curPower;
    }

    // If last step increased power, do bigger step in same direction. Else smaller step opposite direction
    if (curPower < oldPower) {
        stepSize *= -1;
    }

    /* FEATURE 2
    // If last step increased power, do bigger step in same direction. Else smaller step opposite direction
    if (curPower < oldPower) {
        stepSize *= -0.8;
    } else {
        stepSize *= 1.2;
    }

    // Make sure step size not too large, do not allow 0
    if (stepSize > MAX_VOLT_STEP) stepSize = MAX_VOLT_STEP;
    else if (stepSize < -MAX_VOLT_STEP) stepSize = -MAX_VOLT_STEP;
    else if (stepSize > 0 & stepSize < MIN_VOLT_STEP) stepSize = MIN_VOLT_STEP;
    else if (stepSize < 0 & stepSize > -MIN_VOLT_STEP) stepSize = -MIN_VOLT_STEP;
    else if (stepSize == 0) stepSize = MIN_VOLT_STEP;
    */
    
    // Update voltage target for arrays. Do not allow negative
    targetVoltage += stepSize;
    if (targetVoltage <= 0) targetVoltage = 0.01;
    // targetVoltage = 14;
    setVoltOut(targetVoltage);

    // Update power for next cycle
    oldPower = curPower;
}


void initMPPT(std::chrono::microseconds updatePeriod) {
    mpptUpdater.attach(mpptUpdate, updatePeriod);
}
