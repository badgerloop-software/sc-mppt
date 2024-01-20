#ifndef CONST_H
#define CONST_H
// this header should include all relevant constant values so they can all be easily changed


// ------------- IO INPUT CONSTANTS -------------
#define IO_UPDATE_PERIOD 25ms
#define NUM_ARRAYS 3

// Analog input modifiers (derived from circuitry)
constexpr float I_SCALE = 5*3.33;
constexpr float V_SCALE = (103.3/3.3)*3.33;
constexpr float BATT_V_SCALE = 3.325 * 101;

//Input pins to read voltage for each solar array
#define VOLT_IN_1 PA_6
#define VOLT_IN_2 PA_4
#define VOLT_IN_3 PA_1

//Input pins to read current for each solar array
#define CURR_IN_1 PA_7
#define CURR_IN_2 PA_5
#define CURR_IN_3 PA_3

// Input pin to get battery voltage
#define BATTERY_VOLT_IN PB_0

// Output pins for voltage control of arrays via PWM
#define PWM_OUT_1 PA_9
#define PWM_OUT_2 PA_10
#define PWM_OUT_3 PA_8

// duty cycle cannot exceed 0.8
#define DUTY_MAX 0.8


// ------------- MPPT ALGO CONSTANTS -------------
// initial step size of voltage
#define INIT_VOLT_STEP 0.5f

// Frequency of MPPT algo updates
#define MPPT__UPDATE_INTERVAL 300ms


#endif // CONST_H