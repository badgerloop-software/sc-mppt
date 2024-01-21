#include "mbed.h"
#include "canMppt.h"
#include "const.h"
#include "IOManagement.h"
#include "mppt.h"
#include "terminal.h"


// Enables PWM-Voltage converters
DigitalIn boost_en(PB_7);


#if DEBUG_PRINT
void debugPrint() {
    printf("\n");
    for (int i = 0; i < NUM_ARRAYS; i++) {
        printf("Array %i\nVoltage %%5.2fV\t\tCurrent %%5.2fA\nPower %%5.2fW\t\tDuty Cycle %.4f\nStep %.4f\n", 
                i, arrayData[i].voltage, arrayData[i].current, arrayData[i].curPower, 
                arrayData[i].dutyCycle, arrayData[i].step);
    }
}
#endif


int main() {
    initData(IO_UPDATE_PERIOD);
    initMPPT(MPPT_UPDATE_PERIOD);
    CANMPPT canBus(CAN_RX, CAN_TX);

    while (true) {
#if DEBUG_PRINT
        debugPrint();
#endif
        canBus.sendMPPTData();
        canBus.runQueue(DATA_SEND_PERIOD);
    }
}