#include "pid.h"
#include "terminal.h"
#include "mppt.h"
#include "dataManagement.h"
#include "const.h"

MPPT mppt1;
MPPT mppt2;
MPPT mppt3;

PID pid1;
PID pid2;
PID pid3;

static unsigned long long cycles = 0;


DataManagement dm;

void updateVolt(){
    mppt1.voltage = dm.getVoltIn(0);
    mppt2.voltage = dm.getVoltIn(1);
    mppt3.voltage = dm.getVoltIn(2);

}

void updatePrevVolt(){
    mppt1.prevVoltage = mppt1.voltage;
    mppt2.prevVoltage = mppt2.voltage;
    mppt3.prevVoltage = mppt3.voltage;
}

void updateCurr(){
    mppt1.current = dm.getCurrIn(0);
    mppt2.current = dm.getCurrIn(1);
    mppt3.current = dm.getCurrIn(2);

}

void updatePrevCurr(){
    mppt1.prevCurrent = mppt1.current;
    mppt2.prevCurrent = mppt2.current;
    mppt3.prevCurrent = mppt3.current;
}

int main(void) {
    Terminal term;

    int len;
    char buf[BUFFER_SIZE] = {};
    if ((len = term.read(buf)) < 0) exit(1);
    printf("Length of string %s: %d\n", buf, len);


    while (true) {
        updateVolt();
        updateCurr();

        if (cycles++ % MPP_VIN_CYCLES == 0) {
            
        }

        // If power map was just created, set duty to the found mpp_duty. Now after remapping, both vref 
        // and the duty cycle should point each string to the MPP immediately.
        if(cycles % MPP_VIN_CYCLES == 1){

            printf("\nsetting duty cycles to mpp_duty\n\n");

            dm.outputs[0].write(mppt1.dutyCycle);
            dm.outputs[1].write(mppt2.dutyCycle);
            dm.outputs[2].write(mppt3.dutyCycle);
        }
        else{
            mppt1.dutyCycle = incrementalConductance(mppt1, pid1, dm.outputs[0]);
            mppt2.dutyCycle = incrementalConductance(mppt2, pid2, dm.outputs[1]);
            mppt3.dutyCycle = incrementalConductance(mppt3, pid3, dm.outputs[2]);
        }

        updatePrevVolt();
        updatePrevCurr();

        thread_sleep_until(get_ms_count() + CYCLE_MS);
        
    }

}