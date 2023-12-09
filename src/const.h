#ifndef CONST_H
#define CONST_H
// this header should include all relevant constant values so they can all be easily changed

// duty cycle cannot exceed 0.8
#define DUTY_MAX 0.8

//Time to wait after for PID
#define CYCLE_MS 25ms

// step size of voltage
#define VOLTAGE_STEP 0.5f

// number of solar arrays
#define NUM_ARRAYS 3

// port to get battery voltage
#define BATTERY_VOLT_IN PB_0


//Mosfet to adjust circuit voltage for each solar array
#define MOSFET_1 PA_9
#define MOSFET_2 PA_10
#define MOSFET_3 PA_8

//Inputs to read voltage for each solar array
#define VOLT_IN_1 PA_6
#define VOLT_IN_2 PA_4
#define VOLT_IN_3 PA_1
//Inputs to read current for each solar array
#define CURR_IN_1 PA_7
#define CURR_IN_2 PA_5
#define CURR_IN_3 PA_3


#define MPP_VIN_CYCLES 300

//Loop intervals between MPPT updates
#define MPPT_INTERVAL 300

//Number of voltage readings to average
#define MPPT_AVERAGE 3

#define MPPT_STEP

#define TRACK_DELAY 10


#endif // CONST_H