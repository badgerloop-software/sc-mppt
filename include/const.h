#ifndef CONST_H
#define CONST_H
// this header should include all relevant constant values so they can all be easily changed


// ------------- IO INPUT CONSTANTS -------------
#define IO_UPDATE_PERIOD 100ms
#define NUM_ARRAYS 3

// Analog input modifiers (derived from circuitry)
constexpr float I_SCALE = 5*3.33;
constexpr float V_SCALE = (103.3/3.3)*3.33;
constexpr float BATT_V_SCALE = 3.325 * 101;

//Input pins to read voltage for each solar array
#define VOLT_PIN_1 PA_6
#define VOLT_PIN_2 PA_4
#define VOLT_PIN_3 PA_1

//Input pins to read current for each solar array
#define CURR_PIN_1 PA_7
#define CURR_PIN_2 PA_5
#define CURR_PIN_3 PA_3

// Input pin to get battery voltage
#define BATTERY_VOLT_PIN PB_0

// Output pins for voltage control of arrays via PWM
#define PWM_OUT_1 PA_9
#define PWM_OUT_2 PA_10
#define PWM_OUT_3 PA_8

// CAN triggered outputs
#define OV_FAULT_RST_PIN PB_6
#define DISCHARGE_CAPS_PIN PB_1

// Output voltage limit 
#define V_MAX 110

// --------------- PID CONSTANTS -----------------
// Loop parameters
#define P_TERM 0.1
#define I_TERM 0.1
#define D_TERM 0

// Input range
#define PID_IN_MIN 0
#define PID_IN_MAX 115

// PID output limits (controls PWM duty cycles)
#define PWM_DUTY_MIN 0
#define PWM_DUTY_MAX 0.8


// ------------- MPPT ALGO CONSTANTS -------------
// Initial voltage
#define INIT_VOLT 0.0

// initial step size for MPPT updates
#define INIT_VOLT_STEP 0.05

// maximum step size for MPPT updates
#define MAX_VOLT_STEP 2

// Maximum voltage the algo loop can target (want safe offset under danger V_MAX)
#define V_TARGET_MAX 105

// Frequency of MPPT algo updates
#define MPPT_UPDATE_PERIOD 500ms

// Battery voltage threshold to switch to constant current
#define CONST_CURR_THRESH 90

// Battery voltage threshold to switch back to MPPT mode
#define MPPT_THRESH 88

// ------------- TESTING/OTHER CONSTANTS -------------
// Whether to log data and steps in file. Should only be enabled
// when microcontroller outputting via serial 
#define DEBUG_PRINT 0

// How fast to transmit data over CAN (and debug prints if on)
#define DATA_SEND_PERIOD 500ms

// CAN Pins
#define CAN_RX PA_11
#define CAN_TX PA_12

#endif // CONST_H