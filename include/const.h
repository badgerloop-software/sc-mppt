#ifndef CONST_H
#define CONST_H
// this header should include all relevant constant values so they can all be easily changed


// ------------- IO INPUT CONSTANTS -------------
#define IO_UPDATE_PERIOD 100ms
#define NUM_ARRAYS 3

// Analog input modifiers (derived from circuitry)
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

// --------------- PID/PWM CONSTANTS -----------------
// Loop parameters
#define P_TERM -0.1
#define I_TERM -0.1
#define D_TERM 0
#define P_curr_term 0.1
#define I_curr_term 0.1
#define D_curr_term 0

// Input range
#define PID_IN_MIN 0
#define PID_IN_MAX 115

// PID output limits (controls PWM duty cycles)
#define PWM_DUTY_MIN 0
#define PWM_DUTY_MAX 0.8
#define PWM_PERIOD_US 13

// ------------- MPPT ALGO CONSTANTS -------------
// Initial voltage
#define INIT_VOLT 40

// initial step size for MPPT updates
#define INIT_VOLT_STEP 0.5

// step size limits for MPPT updates
#define MIN_VOLT_STEP 0.01
#define MAX_VOLT_STEP 2

// Maximum voltage the algo loop can target (want safe offset under danger V_MAX)
#define V_TARGET_MAX 105

//Maximum current the algo loop can target would be the pack charge current limit
#define I_TARGET_MAX 20

// Frequency of MPPT algo updates
#define MPPT_UPDATE_PERIOD 500ms

// Current threshold to switch to constant current
#define CONST_CURR_THRESH packChargeCurrentLimit

// Current threshold to switch to MPPT
#define MPPT_THRESH packChargeCurrentLimit-2

#define CONST_CURR_SAFETY_MULT 0.9

// ------------- TESTING/OTHER CONSTANTS -------------
// Whether to log data and steps in file. Should only be enabled
// when microcontroller outputting via serial 
#define DEBUG_PRINT 1

// How fast to transmit data over CAN (and debug prints if on)
#define DATA_SEND_PERIOD 50ms

// CAN Pins
#define CAN_RX PA_11
#define CAN_TX PA_12

// Duration undervoltage fault reset asserted on command 
#define OV_FAULT_RST_PERIOD 250ms

#endif // CONST_H