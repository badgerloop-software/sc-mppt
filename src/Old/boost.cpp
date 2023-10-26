#include "boost.h"
#include "mbed.h"

PID::PID(float pterm, float iterm, float dterm, PinName p)
    : p_(pterm), i_(iterm), d_(dterm), perror_(0), pwm_(FastPWM(p)),
      integral_(0) {
  pwm_.period_us(13);
  pwm_.write(0);
}

float PID::duty(float desired, float now, float max, uint64_t change) {
  float dt = change / (float)CYCLE_MS.count();
  float error = (now - desired) / max;
  integral_ += error * dt;
  float derivative = ((error - perror_) / (float)dt);
  float duty = p_ * error + i_ * integral_;
  
  // A variable to represent duty cycle which isn't bounded between 0 and 1. 
  // Can give insight to why duty cycles remain at 0 or 1.
  float duty_tester = p_ * error + i_ * integral_;
  
  // static int tmp_duty = 0;
  // tmp_duty = (tmp_duty+1)%101;
  // duty = tmp_duty/(float)100;
  duty = (duty < 0) ? 0 : ((duty > 0.8) ? 0.8 : duty);
  perror_ = error;
  
  // If voltage or current of a string being controlled is zero, set duty cycle to 0.
  // This may or may not always work e.g. 0.0 dc may indefinitely force a string's 
  // current to 0 when current is being tracked.
  if (now >= 0.05) {
  duty = 0.0;
  }  
// Comment/uncomment for manually set duty cycle
  else{
  duty = 0.0;
  }

  pwm_.write(duty);
  //printf("Unbounded DC: %.3f | DC: %.3f | Now: %05.2f | Ref: %05.2f | Error: %05.2f | Integral: %05.2f | dt: %05.2f | p_: %05.2f | i_: %05.2f\n", duty_tester, duty, now, desired, error, integral_,dt, p_, i_);
  
  return duty;
}

void PID::reset() {
  perror_ = 0;
  integral_ = 0;
}

BoostConverter::BoostConverter(PinName v, PinName i, PinName p)
    : voltageADC_(AnalogIn(v)), currentADC_(AnalogIn(i)),
      pid(PID(PTERM, ITERM, DTERM, p)), pp_(0), dir_(true) {}


//TODO add to i/o management class
float BoostConverter::getIin(void) { return currentADC_.read()*3.33*I_SCALE; }
//  Declan's Equation : read()*3.3*0.943+0.0294;
float BoostConverter::getVin(void) { return voltageADC_.read()*3.33*V_SCALE; }

// Take in sample of voltages/current (3) and compares to previous power to appropriately change voltage
float BoostConverter::PO(float vin, float iin) {
  float power = vin * iin;
  if (power < pp_)
    dir_ = !dir_;
  pp_ = power;
  return dir_ ? PO_VOLTAGE_STEP : -1 * PO_VOLTAGE_STEP;
}
