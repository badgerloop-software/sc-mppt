#include "IOManagement.h"


// Solar array voltage, current, and PWM pins (controlled by PID) and storage variable
struct ArrayPins {
    AnalogInMutexless voltPin;
    AnalogInMutexless currPin;
    PID pidController;
    FastPWM pwmPin;
};

ArrayPins arrayPins[NUM_ARRAYS] = {
    {AnalogInMutexless(VOLT_PIN_1), AnalogInMutexless(CURR_PIN_1), PID(P_TERM, I_TERM, D_TERM, (float)IO_UPDATE_PERIOD.count() / 1000), FastPWM(PWM_OUT_1)},
    {AnalogInMutexless(VOLT_PIN_2), AnalogInMutexless(CURR_PIN_2), PID(P_TERM, I_TERM, D_TERM, (float)IO_UPDATE_PERIOD.count() / 1000), FastPWM(PWM_OUT_2)},
    {AnalogInMutexless(VOLT_PIN_3), AnalogInMutexless(CURR_PIN_3), PID(P_TERM, I_TERM, D_TERM, (float)IO_UPDATE_PERIOD.count() / 1000), FastPWM(PWM_OUT_3)}
};

volatile ArrayData arrayData[NUM_ARRAYS];


// Battery voltage pin and storage
AnalogInMutexless batteryVoltIn(BATTERY_VOLT_PIN);
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


// Updates arrayData with new input values and PWM outputs based on PID loop
void updateData() {
    for (int i = 0; i < NUM_ARRAYS; i++) {
        // Update temperature mux selection at start for time to update, then read at end
        // Inputs corresponds to bits 0 and 1 of array number
        thermMuxSel0.write(i & 0x1);
        thermMuxSel1.write(i & 0x2);
        arrayData[i].voltage = arrayPins[i].voltPin.read() * V_SCALE;
        arrayData[i].current = arrayPins[i].currPin.read() * I_SCALE;
        arrayData[i].lastPower = arrayData[i].curPower;
        arrayData[i].curPower = arrayData[i].voltage * arrayData[i].current;
        arrayData[i].dutyCycle = arrayPins[i].pwmPin.read();
        arrayData[i].temp = thermPin.read();

        // Output duty cycle update, shut off if over threshold
        if (arrayData[i].voltage > V_MAX) {
            arrayPins[i].pwmPin.write(0);
        } else {
            arrayPins[i].pidController.setProcessValue(arrayData[i].voltage);
            arrayPins[i].pwmPin.write(arrayPins[i].pidController.compute());
        }
        
    }

    battVolt = batteryVoltIn.read() * BATT_V_SCALE;
    if (battVolt > CONST_CURR_THRESH) chargeMode = ChargeMode::CONST_CURR;
    else if (battVolt < MPPT_THRESH) chargeMode = ChargeMode::MPPT;
}


void initData(std::chrono::microseconds updatePeriod) {
    // Auto updating IO
    dataUpdater.attach(updateData, updatePeriod);

    // PID loop setup
    for (int i = 0; i < NUM_ARRAYS; i++) {
        arrayPins[i].pidController.setInputLimits(PID_IN_MIN, PID_IN_MAX);
        arrayPins[i].pidController.setOutputLimits(PWM_DUTY_MIN, PWM_DUTY_MAX);
        arrayPins[i].pidController.setOutputLimits(PWM_DUTY_MIN, PWM_DUTY_MAX);
        arrayPins[i].pidController.setMode(AUTO_MODE);
        arrayPins[i].pidController.setSetPoint(INIT_VOLT);
    }
}


void setVoltOut(uint8_t arrayNumber, float voltage) {
    if (voltage > V_TARGET_MAX) voltage = V_TARGET_MAX;
    arrayPins[arrayNumber].pidController.setSetPoint(voltage);
}

void setOVFaultReset(uint8_t value) {
    OVFaultReset.write(value);
}


void setCapDischarge(uint8_t value) {
    capDischarge.write(value);
}