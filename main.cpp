#include "mbed.h"
#include "canMppt.h"
#include "const.h"
#include "IOManagement.h"
#include "mppt.h"
#include "terminal.h"


#if DEBUG_PRINT == 1
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
    printf("Target Voltage: %f\n", targetVoltage);
    // Compute current output current for feedback
    float totalInputPower = 0;
    for (int i = 0; i < NUM_ARRAYS; i++) {
        totalInputPower += arrayData[i].curPower;
    }
    float outputCurr = totalInputPower / battVolt;
    printf("Output Current: %f\n", outputCurr);
}
#elif DEBUG_PRINT == 2
void debugPrint() {
    printf("%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f\n",
            arrayData[0].voltage, arrayData[0].current,arrayData[0].temp,
            arrayData[1].voltage, arrayData[1].current,arrayData[1].temp,
            arrayData[2].voltage, arrayData[2].current,arrayData[2].temp,
            battVolt, targetVoltage);
}
#endif

int main() {
#if DEBUG_PRINT
    BufferedSerial serial(USBTX, USBRX, 115200);
    serial.set_blocking(false);
    int counter = 0;
    char buf[1];
#endif
#if DEBUG_PRINT == 2
    printf("voltage0,current0,temp0,voltage1,current1,temp1,voltage2,current2,temp2,battVolt,targVolt\n");
#endif


    initData(IO_UPDATE_PERIOD);
    initMPPT(MPPT_UPDATE_PERIOD);
    CANMPPT canBus(CAN_RX, CAN_TX);

    while (true) {
#if DEBUG_PRINT
        // Display digital and analog values every second (for testing) 
        if (counter >= (1000 / DATA_SEND_PERIOD.count())) {
            //printf("%f\n", arrayData[0].voltage);
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