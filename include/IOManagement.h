#ifndef __IO_MANAGEMENT_H__
#define __IO_MANAGEMENT_H__

#include "mbed.h"
#include "const.h"
#include "FastPWM.h"
#include "mutexless_analog.h"


// Solar array and battery data
typedef struct ArrayData {
    double dutyCycle = INIT_VOLT;
    float voltage = 0;
    float current = 0;
    float curPower = 0;
    float lastPower = 0;
    float step = INIT_VOLT_STEP;
    float temp = 0;
} ArrayData;

extern volatile ArrayData arrayData[NUM_ARRAYS];
extern volatile float battVolt;

// Charging algorithm mode
enum class ChargeMode : bool {CONST_CURR, MPPT};
extern volatile ChargeMode chargeMode;

// Sets up automatic updating of arrayData
// at specified period
void initData(std::chrono::microseconds updatePeriod);


// Sets PWM output for specified array
// Duty cycle should be a value between 0 and DUTY_MAX Values outside will be capped
void setPWMOut(uint8_t arrayNumber, double dutyCycle);

// Sets clearing of OV fault
void setOVFaultReset(uint8_t value);

// Controls discharging of output capacitors thorugh resistor path
void setCapDischarge(uint8_t value);

#endif // __IO_MANAGEMENT_H__