#include "pid.h"
#include "terminal.h"
#include "mppt.h"
#include "dataManagement.h"
#include "const.h"



static unsigned long long cycles = 0;
static int tracking = TRACK_DELAY;
int mppt_calc = MPPT_AVERAGE;

DataManagement dm;

DigitalIn boost_en(PB_7);


void mppt_INCCOND()
{
    //Run increment conductance for all solar arrays
    for (int i = 0; i < NUM_ARRAYS; i++){
        long delta_p = dm.getPower(i) - dm.getPrevPower(i);

        long delta_i = (long) dm.getCurrAcc(i) - dm.getPrevCurr(i);
        long delta_v = (long) dm.getVoltAcc(i) - dm.getPrevVolt(i);

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

// void cc_cv_mode()
// {
// 	if(vout > vref) 
// 	{
// 		if(cc_cv) cc_cv--; else
// 		{
// 			if(cmode) pi = 0;
// 			cmode = 0;
// 		}
// 	}
// 	if(iout > iref) 
// 	{
// 			if(!cmode) pi = 0;
// 			cmode = 1;
// 			cc_cv = CURRENT_MODE;
// 	}
// }

typedef std::chrono::duration<long long, std::ratio<1, 1000>> millisecond;

millisecond get_ms() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      Kernel::Clock::now().time_since_epoch());
}

// POWER MAPPING
// This method is intermittently used to remap the power curve at a given time by cycling through duty cycles.
// The purpose is to ensure the algorithm is at the correct MPP in case a "local" MPP is found outside of the
// true MPP.
// TO DO: Figure out how this can return three floats mpp_vin0, mpp_vin1, and mpp_vin2
    void power_map() {
        // The maximum power needs to be reset before each remap.
        float mpow[3] = {0,0,0};

        // Room for improvement: The method currently always cycles through a specified duty cycle range.
        // The more the duty cycle varies from normal operation, the less efficient the algorithm is. There
        // may be a way to adjust a smaller range according to where the last MPP was found.
        for(int i = 5; i <= 20; i++) {
        
            for (int j = 0; j < NUM_ARRAYS; j++) {
                dm.outputs[j].write(i * (float)1 / 50);
            }
        
            ThisThread::sleep_for(CYCLE_MS);

            //Gets a new current and volatage from each array, and battery volt
            dm.read_ADC();

            //Prints volatage, power, and current values to file (see TxtData folder)
            printf("   DC:%5.2f   V: %5.2fV | %5.2fV | %5.2fV   Vout: %5.2fV P: %5.2fW | %5.2fW | %5.2fW   MPP V: %5.2fV | %5.2fV | %5.2fV   MPP P: %5.2fW | %5.2fW | %5.2fW   BE: %i\n",
            i * (float)1 / 50, dm.mppts[0].voltage, dm.mppts[1].voltage, dm.mppts[2].voltage,
             dm.battVolt, dm.mppts[0].power,dm.mppts[1].power, dm.mppts[0].power,
              dm.mppts[0].targetVolt, dm.mppts[1].targetVolt, dm.mppts[2].targetVolt, 
              mpow[0], mpow[1], mpow[2]);
        
            for (int j = 0; j < NUM_ARRAYS; j++) {
                if (dm.mppts[j].power > mpow[j]) {
                    dm.mppts[j].targetVolt = dm.mppts[j].voltage;
                    mpow[j] = dm.mppts[j].voltage;
                    dm.mppts[j].dutyCycle = i * (float)1 / 50;
                }
            
            }
        }

    //Print the duty, max power, and mpp volate in
    printf("/////////////////////////////\n// TARGET:\n//\n//  MPP DC:%5.2f |%5.2f |%5.2f\n//  MPP POWER: %5.2fW | %5.2fW | %5.2fW\n//  MPP Vin: %5.2f | %5.2f | %5.2f\n//\n/////////////////////////////\n",
    dm.mppts[0].dutyCycle, dm.mppts[1].dutyCycle, dm.mppts[2].dutyCycle,
     mpow[0], mpow[1], mpow[2], dm.mppts[0].targetVolt, dm.mppts[1].targetVolt, dm.mppts[2].targetVolt);


    ThisThread::sleep_for(CYCLE_MS);
}


int main(void) {
    Terminal term;

    int len;
    char buf[BUFFER_SIZE] = {};
    if ((len = term.read(buf)) < 0) exit(1);
    printf("Length of string %s: %d\n", buf, len);


    millisecond current_time, p_time = get_ms();  
//Old method
    while (true) {
    
        if (cycles++ % MPP_VIN_CYCLES == 0) {
            power_map();
            p_time = get_ms();

        }

        thread_sleep_until((p_time + CYCLE_MS).count());
        current_time = get_ms();
        //Read all voltage/current data
        dm.read_ADC();

        // If power map was just created, set duty to the found mpp_duty. Now after remapping, both vref 
        // and the duty cycle should point each string to the MPP immediately.
        
        if(mppt_calc < MPPT_AVERAGE){
            dm.incCurr();
            dm.incVolt();
        }

        if(!mppt_calc){
            mppt_calc = MPPT_INTERVAL;
            for (int i = 0; i < NUM_ARRAYS; i++) {
                mppt_INCCOND();
            }
        }else{
            mppt_calc--;
        }


        if(cycles % MPP_VIN_CYCLES == 1){

            printf("\nsetting duty cycles to mpp_duty\n\n");

            for (int i = 0; i < NUM_ARRAYS; i++) {
                dm.outputs[i].write(dm.mppts[i].dutyCycle);
            }
        }
        else{
            for (int i = 0; i < NUM_ARRAYS; i++) {
                dm.mppts[i].dutyCycle = updatePID(dm.pids[i], dm.mppts[i].targetVolt, dm.mppts[i].voltage, (current_time - p_time).count());
            }
        }

        dm.updatePrevVolt();
        dm.updatePrevCurr();

        thread_sleep_until((p_time + CYCLE_MS).count());
        p_time = current_time;

        float totalPower = (dm.mppts[0].power + dm.mppts[1].power + dm.mppts[2].power);
        float iout = totalPower/dm.battVolt;

        if(tracking){
            printf("DC:%5.2f /%5.2f /%5.2f   Vin: %5.2fV / %5.2fV / %5.2fV   Vref: %5.2fV / %5.2fV / %5.2fV   Vout: %5.2fV   Iin:%5.2fA /%5.2fA /%5.2fA   Iout:%5.2fA   Pin:%5.2fW   BE:%i\n",
            dm.mppts[0].dutyCycle,dm.mppts[1].dutyCycle,dm.mppts[2].dutyCycle, 
            dm.mppts[0].voltage, dm.mppts[1].voltage, dm.mppts[2].voltage, 
            dm.mppts[0].targetVolt, dm.mppts[1].targetVolt, dm.mppts[2].targetVolt,
            dm.battVolt,dm.mppts[0].current,dm.mppts[1].current,dm.mppts[2].current,iout,totalPower,boost_en.read());
        }

        
    }


    //Alternative method purposed in document (see MPPT chat)
    //Initialize_Hardware();

    // int T0IF = 0;
    // int but_cnt = 0;
    // int track = 0;
    // int second = 0;
    // int mppt_calc = 0;
    // int battery_state = 1;
    // float dmax = VOLTAGE_STEP;
    // //Target output voltage
    // float vref = 0;
    // //Target output current
    // float iref = 0;


    // while (true)
    // {
    //     if(T0IF)
    //     {
    //         T0IF = 0;
    //         if(but_cnt) but_cnt--;
    //         if(track && mppt_calc) mppt_calc--;
    //         if(second) second--;
    //         dm.read_ADC();

    //     if(!track) {
    //         // if(battery_state != FAULT){
    //             // cc_cv_mode();
    //             // if(!cmode) pid(vout, vref); else
    //             // pid(iout, iref);
    //             // if(increment >= dmax) track = TRACK_DELAY;
    //         // }
    //     } else { 
            
    //         //Use voltage as target (could create case with current)
            
    //         if(VOLTAGE_STEP >= dmax) track = TRACK_DELAY;

    //         if(mppt_calc < MPPT_AVERAGE){
    //             dm.incCurr();
    //             dm.incVolt();
    //         }

    //         if(!mppt_calc)
    //         {
    //             mppt_calc = MPPT_INTERVAL;
    //             mppt_INCCOND();
                
    //         }

    //         for (int i = 0; i < NUM_ARRAYS; i++) {
    //             updatePID(dm.pids[i], dm.mppts[i].targetVolt, dm.getVolt(i));
    //         }

    //         if(dm.getBattVolt() > vref || iout > iref)
    //         {
    //             track--;
    //             dmax = VOLTAGE_STEP;
    //             if(!track) Init_State_Machine();
    //         } else
    //             track = TRACK_DELAY;
            

    //         if(!second)
    //         {
    //             second = SECOND_COUNT;
    //             if(!track) Battery_State_Machine();
    //         }
    //     }
    // }


}