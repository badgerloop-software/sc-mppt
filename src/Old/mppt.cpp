#include "mppt.h"
#include "mbed.h"

static CAN c(PA_11, PA_12, 500000);

Mppt::Mppt(void) /* voltage current pwm */
    : batteryADC_(AnalogIn(PB_0)), bc0(BoostConverter(PA_6, PA_7, PA_9)),
      bc1(BoostConverter(PA_4, PA_5, PA_10)),
      bc2(BoostConverter(PA_1, PA_3, PA_8)), 
      
      can_(&c) {}

Mppt::~Mppt(void) {
  running_ = false;
  thread_.join();
}

//Add to i/o management class
float Mppt::getVout(void) { return batteryADC_.read() * 3.325 * 101; }


//Handles can message dont worry about for now
bool Mppt::notInit(void) {
  if (!running_) {
    running_ = true;
    if (thread_.start(callback(this, &Mppt::canLoop)) != osOK)
      running_ = false;
  }
  return !running_;
}

//Handles can message dont worry about for now
void Mppt::canLoop(void) {
  CANMessage msg;
  while (running_) {
    if (can_->read(msg)) {
      parse(msg);
    }
    ThisThread::sleep_for(200ms);
  }
}

//Handles can message dont worry about for now
void Mppt::parse(CANMessage &msg) {
  switch (msg.id) {

  case MPPT_MOC_ID:
    maxIout.setValue(msg.data);
    printf("set maxIout to %.3f", *(float *)(msg.data));
    break;
  }
}

// int Mppt::send(unsigned int id, char *data, unsigned int len){
//     // if(len > 8){
//     //     return -1;
//     // }
//     // CANMessage msg(id, data, len);
//     // return !(;

    
// }