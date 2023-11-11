#ifndef CONST_H
#define CONST_H
// this header should include all relevant constant values so they can all be easily changed

// duty cycle cannot exceed 0.8
#define DUTY_MAX 0.8

// size of timestep
#define CYCLE_MS 25

// step size of voltage
#define VOLTAGE_STEP 0.5f

// number of solar arrays
#define NUM_ARRAYS 3

// port to get battery voltage
#define BATTERY_VOLT_IN PB_0

#define MOSFET_1 PA_9
#define MOSFET_2 PA_10
#define MOSFET_3 PA_8

#define MPP_VIN_CYCLES 300


#endif // CONST_H