#include "mbed.h"
#include "canMppt.h"
#include "const.h"
#include "IOManagement.h"
#include "mppt.h"
#include "terminal.h"


#if DEBUG_PRINT
void debugPrint() {
    printf("\n");
    for (int i = 0; i < NUM_ARRAYS; i++) {
        printf("Array %i\nVoltage %5.2fV\t\tCurrent %5.2fA\nPower %5.2fW\t\tDuty Cycle %.4f\nTemp %.4f\n", 
                i+1, arrayData[i].voltage, arrayData[i].current, arrayData[i].curPower, 
                arrayData[i].dutyCycle, arrayData[i].temp);
    }
    printf("Boost enable: %i\nBattery Voltage: %5.2f\n", boostEnabled, battVolt);
}
#endif

int main() {
    BufferedSerial term(USBTX, USBRX, 115200);
    char buf[1] = {'a'};

    initData(100ms);

    while (true) {
        // wait for keyboard input to start test
        printf("Waiting to begin\n");
        
        while (buf[0] != 's') {
            term.read(buf, 1);
        }
        buf[0] = 0;
        printf("duty,voltage0,current0,voltage1,current1,voltage2,current2\n");

        // increment mosfet output by .01 every 0.5 seconds and record data
        for (float pwm_value = 0.0; pwm_value <= 0.5; pwm_value += .01) {
            // set mosfet outputs
            benchmarkUpdatePWM(pwm_value);
            // wait 0.5s for data
            wait_us(500000);
            // print out voltage and current
            printf("%f,%f,%f,%f,%f,%f,%f\n", 
                    pwm_value,
                    arrayData[0].voltage,
                    arrayData[0].current,
                    arrayData[1].voltage,
                    arrayData[1].current,
                    arrayData[2].voltage,
                    arrayData[2].current);
        }
        benchmarkUpdatePWM(0);
    }

}