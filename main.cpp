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
#if DEBUG_PRINT
    BufferedSerial serial(USBTX, USBRX, 115200);
    int counter = 0;
#endif


    initData(IO_UPDATE_PERIOD);
    initMPPT(MPPT_UPDATE_PERIOD);
    CANMPPT canBus(CAN_RX, CAN_TX);

    while (true) {
#if DEBUG_PRINT
        // Display digital and analog values every second (for testing) 
        if (counter >= (1000 / DATA_SEND_PERIOD.count())) {
            debugPrint();
            counter = 0;
        }
        counter++;
#endif

        canBus.sendMPPTData();
        canBus.runQueue(DATA_SEND_PERIOD);
    }
}