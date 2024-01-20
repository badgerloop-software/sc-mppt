#include "IOManagement.h"


// Solar array voltage, current, and PWM pins and storage variable
struct ArrayPins {
    AnalogInMutexless voltPin;
    AnalogInMutexless currPin;
    FastPWM pwmPin;
};

ArrayPins arrayInputs[NUM_ARRAYS] = {
    {AnalogInMutexless(VOLT_IN_1), AnalogInMutexless(CURR_IN_1), FastPWM(PWM_OUT_1)},
    {AnalogInMutexless(VOLT_IN_2), AnalogInMutexless(CURR_IN_2), FastPWM(PWM_OUT_2)},
    {AnalogInMutexless(VOLT_IN_3), AnalogInMutexless(CURR_IN_3), FastPWM(PWM_OUT_3)}
};

volatile ArrayData arrayData[NUM_ARRAYS];

// Battery voltage pin and storage
AnalogInMutexless batteryVoltIn(BATTERY_VOLT_IN);
volatile float battVolt;

// Ticker to poll input readings at fixed rate
Ticker dataUpdater;


void updateData() {
    for (int i = 0; i < NUM_ARRAYS; i++) {
        arrayData[i].voltage = arrayInputs[i].voltPin.read() * V_SCALE;
        arrayData[i].current = arrayInputs[i].currPin.read() * I_SCALE;
        arrayData[i].power = arrayData[i].voltage * arrayData[i].current;
        arrayData[i].dutyCycle = arrayInputs[i].pwmPin.read();
    }

    battVolt = batteryVoltIn.read() * BATT_V_SCALE;
}


void initData(std::chrono::microseconds updatePeriod) {
    dataUpdater.attach(updateData, updatePeriod);
}


void setPWMOut(uint8_t arrayNumber, double dutyCycle) {
    if (dutyCycle > DUTY_MAX) dutyCycle = DUTY_MAX;
    arrayInputs[arrayNumber].pwmPin.write(dutyCycle);
}