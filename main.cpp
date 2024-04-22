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
    printf("Mode: %s\n", (bool)chargeMode ? "MPPT" : "Current");
    printf("Current Limit: %f\n", packChargeCurrentLimit);
    // Compute current output current for feedback
    float totalInputPower = 0;
    for (int i = 0; i < NUM_ARRAYS; i++) {
        totalInputPower += arrayData[i].curPower;
    }
    float outputCurr = totalInputPower / battVolt;
    printf("Output Current: %f\n", outputCurr);
}
#endif

int main() {
#if DEBUG_PRINT
    BufferedSerial serial(USBTX, USBRX, 115200);
    serial.set_blocking(false);
    int counter = 0;
    char buf[1];
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

        if (serial.read(buf, 1) > 0) {
            if (buf[0] == 'c') packChargeCurrentLimit = 2;
            else if (buf[0] == 'm') packChargeCurrentLimit = 80;
            buf[0] = 0;
        }
#endif

        canBus.sendMPPTData();
        canBus.runQueue(DATA_SEND_PERIOD);
    }
}