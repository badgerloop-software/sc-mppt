#include "mbed.h"
#include "const.h"
#include "IOManagement.h"
#include "mppt.h"
#include "terminal.h"


// Enables PWM-Voltage converters
DigitalIn boost_en(PB_7);


int main() {
    Terminal term;
    initData(IO_UPDATE_PERIOD);
    initMPPT(MPPT_UPDATE_PERIOD);

    while (true) {

    }
}