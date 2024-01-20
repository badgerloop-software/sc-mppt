#ifndef __IO_MANAGEMENT_H__
#define __IO_MANAGEMENT_H__

#include "mbed.h"
#include "const.h"
#include "mppt.h"
#include "mutexless_analog.h"


// Solar array and battery data
typedef struct ArrayData {
    float voltage;
    float current;
    float power;
    float dutyCycle;
} ArrayData;

extern volatile ArrayData arrayData[NUM_ARRAYS];
extern volatile float battVolt;


// Sets up automatic updating of arrayData
// at specified period
void initData(std::chrono::microseconds updatePeriod);


// Sets PWM output for specified array
// Duty cycle should be a value between 0 and DUTY_MAX Values outside will be capped
void setPWMOut(uint8_t arrayNumber, float dutyCycle);

#endif // __IO_MANAGEMENT_H__