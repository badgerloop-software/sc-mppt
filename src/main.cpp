#include "pid.h"
#include "terminal.h"
#include "mppt.h"
#include "dataManagement.h"
#include "const.h"



static unsigned long long cycles = 0;


DataManagement dm;

void mppt_INCCOND()
{
    for (int i = 0; i < NUM_ARRAYS; i++){
        dm.updatePow();
        long delta_p = dm.getPower(i) - dm.getPrevPower(i);

        long delta_i = (long) dm.getCurr(i) - dm.getPrevCurr(i);
        long delta_v = (long) dm.getVolt(i) - dm.getPrevVolt(i);

        if(delta_v)
        {
            long ineq = delta_p / delta_v;
            if(ineq > 0) dm.mppts[i].targetVolt += VOLTAGE_STEP; else if(ineq < 0) dm.mppts[i].targetVolt -= VOLTAGE_STEP;
        } else {
            if(delta_i > 0) dm.mppts[i].targetVolt += VOLTAGE_STEP; else if(delta_i < 0) dm.mppts[i].targetVolt -= VOLTAGE_STEP;
        }

    }

    dm.updatePrevCurr();
    dm.updatePrevVolt();
    dm.updatePrevPow();

    dm.zeroCurr();
    dm.zeroVolt();
}

int main(void) {
    Terminal term;

    int len;
    char buf[BUFFER_SIZE] = {};
    if ((len = term.read(buf)) < 0) exit(1);
    printf("Length of string %s: %d\n", buf, len);


//Old method
    // while (true) {
    //     dm.updateVolt();
    //     dm.updateCurr();

    //     if (cycles++ % MPP_VIN_CYCLES == 0) {
            
    //     }

    //     // If power map was just created, set duty to the found mpp_duty. Now after remapping, both vref 
    //     // and the duty cycle should point each string to the MPP immediately.
    //     if(cycles % MPP_VIN_CYCLES == 1){

    //         printf("\nsetting duty cycles to mpp_duty\n\n");

    //         for (int i = 0; i < NUM_ARRAYS; i++) {
    //             dm.outputs[i].write(dm.mppts[i].dutyCycle);
    //         }
    //     }
    //     else{
    //         for (int i = 0; i < NUM_ARRAYS; i++) {
    //             dm.mppts[i].dutyCycle = incrementalConductance(dm.mppts[i], dm.pids[i], dm.outputs[i]);
    //         }
    //     }

    //     dm.updatePrevVolt();
    //     dm.updatePrevCurr();

    //     thread_sleep_until(get_ms_count() + CYCLE_MS);
        
    // }

    Initialize_Hardware();

    int T0IF = 0;
    int but_cnt = 0;
    int track = 0;
    int second = 0;
    int mppt_calc = 0;
    int battery_state = 1;

    while(true)
    {
        if(T0IF)
        {
            T0IF = 0;
            if(but_cnt) but_cnt--;
            if(track && mppt_calc) mppt_calc--;
            if(second) second--;
            dm.read_ADC();

        if(!track) {
            if(battery_state != FAULT){

            }
        } else { 
            cc_cv_mode();
            if(!cmode){
                for (int i = 0; i < NUM_ARRAYS; i++) {
                    updatePID(dm.pids[i], vout, dm.mppts[i].targetVolt[i]);
                }
            }  else
            pid(iout, iref);
            if(increment >= dmax) track = TRACK_DELAY;
            if(mppt_calc < MPPT_AVERAGE){}
            
            dm.incCurr();
            dm.incVolt();

            if(!mppt_calc)
            {
                mppt_calc = MPPT_INTERVAL;
                mppt_INCCOND();
                
            }


            pid(vinref, vin);
            if(vout > vref || iout > iref)
            {
                track--;
                dmax = increment;
                if(!track) Init_State_Machine();
            } else
                track = TRACK_DELAY;
            }

            if(!second)
            {
                second = SECOND_COUNT;
                if(!track) Battery_State_Machine();
            }
        }
    }


}