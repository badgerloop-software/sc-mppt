#include "IOManagement.h"


// PWM output pins (declared outside array since non-copyable)
AnalogOutMutexless pwmPin1(PWM_OUT_1);
AnalogOutMutexless pwmPin2(PWM_OUT_2);
AnalogOutMutexless pwmPin3(PWM_OUT_3);

// Solar array voltage/current pins and storage
struct ArrayPins {
    AnalogInMutexless voltPin;
    AnalogInMutexless currPin;
    AnalogOutMutexless& pwmPin;
};

ArrayPins arrayInputs[NUM_ARRAYS] = {
    {AnalogInMutexless(VOLT_IN_1), AnalogInMutexless(CURR_IN_1), pwmPin1},
    {AnalogInMutexless(VOLT_IN_2), AnalogInMutexless(CURR_IN_2), pwmPin2},
    {AnalogInMutexless(VOLT_IN_3), AnalogInMutexless(CURR_IN_3), pwmPin3}
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


void setPWMOut(uint8_t arrayNumber, float dutyCycle) {
    if (dutyCycle > DUTY_MAX) dutyCycle = DUTY_MAX;
    arrayInputs[arrayNumber].pwmPin.write(dutyCycle);
}