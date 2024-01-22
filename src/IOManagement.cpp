#include "IOManagement.h"
#include <array>


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

// Temperature reading pins (single ADC, select thermistor via multiplexer)
DigitalOut thermMuxSel0(PB_5);
DigitalOut thermMuxSel1(PB_4);
AnalogInMutexless thermPin(PA_0);

// Misc controlled outputs. Default to nominal state
DigitalOut OVFaultReset(OV_FAULT_RST_PIN, 0);
DigitalOut capDischarge(DISCHARGE_CAPS_PIN, 1);

// Charging algorithm mode
volatile ChargeMode chargeMode = ChargeMode::MPPT;

// Ticker to poll input readings at fixed rate
Ticker dataUpdater;


void updateData() {
    for (int i = 0; i < NUM_ARRAYS; i++) {
        // Update mux selection at start for time to update, then read at end
        // Inputs corresponds to bits 0 and 1 of array number
        thermMuxSel0.write(i & 0x1);
        thermMuxSel1.write(i & 0x2);
        arrayData[i].voltage = arrayInputs[i].voltPin.read() * V_SCALE;
        arrayData[i].current = arrayInputs[i].currPin.read() * I_SCALE;
        arrayData[i].lastPower = arrayData[i].curPower;
        arrayData[i].curPower = arrayData[i].voltage * arrayData[i].current;
        arrayData[i].dutyCycle = arrayInputs[i].pwmPin.read();
        arrayData[i].temp = thermPin.read();
    }

    battVolt = batteryVoltIn.read() * BATT_V_SCALE;
    if (battVolt > CONST_CURR_THRESH) chargeMode = ChargeMode::CONST_CURR;
    else if (battVolt < MPPT_THRESH) chargeMode = ChargeMode::MPPT;
}


void initData(std::chrono::microseconds updatePeriod) {
    dataUpdater.attach(updateData, updatePeriod);
}


void setPWMOut(uint8_t arrayNumber, double dutyCycle) {
    if (dutyCycle > DUTY_MAX) dutyCycle = DUTY_MAX;
    arrayInputs[arrayNumber].pwmPin.write(dutyCycle);
}

void setOVFaultReset(uint8_t value) {
    OVFaultReset.write(value);
}


void setCapDischarge(uint8_t value) {
    capDischarge.write(value);
}