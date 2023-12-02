#include "dataManagement.h"
#include "const.h"

DataManagement::DataManagement() {}

//Gets new voltage from specificied solar array
float DataManagement::getVoltIn(int arrayIdx){
    return inputs[arrayIdx].vIn.read()*V_SCALE;
}

//Gets the current of specificied solar array
float DataManagement::getCurrIn(int arrayIdx){
    // Declan's Equation : read()*3.3*0.943+0.0294;
    return inputs[arrayIdx].iIn.read()*I_SCALE;
}

// //Calculates the power of specificied solar array
// float DataManagement::getPower(int arrayIdx){
//     return prevData[arrayIdx].power = prevData[arrayIdx].voltIn * prevData[arrayIdx].currIn;
// }

//Updates voltage for each solar array
void DataManagement::incVolt(){
    for (int i = 0; i < NUM_ARRAYS; i++) {
        mppts[i].voltage += getVoltIn(i);
    }
}

//Updates current for each solar array
void DataManagement::incCurr(){
    for (int i = 0; i < NUM_ARRAYS; i++) {
        mppts[i].voltage += getVoltIn(i);
    }
}

//Updates previous voltage variable for each solar array
void DataManagement::updatePrevVolt(){
    for (int i = 0; i < NUM_ARRAYS; i++) {
        mppts[i].prevVoltage = mppts[i].voltage;
    }
}

//Updates previous current variable for each solar array
void DataManagement::updatePrevCurr(){
    for (int i = 0; i < NUM_ARRAYS; i++) {
        mppts[i].prevCurrent = mppts[i].current;
    }
}

float DataManagement::getVolt(int array_num){
    return mppts[array_num].voltage;
}

float DataManagement::getPrevVolt(int array_num){
    return mppts[array_num].prevVoltage;
}

float DataManagement::getCurr(int array_num){
    return mppts[array_num].current;
}

float DataManagement::getPrevCurr(int array_num){
    return mppts[array_num].prevCurrent;
}

float DataManagement::getPower(int array_num){
    return mppts[array_num].power;
}

float DataManagement::getPrevPower(int array_num){
    return mppts[array_num].prevPower;
}

void DataManagement::zeroVolt(){
    for (int i = 0; i < NUM_ARRAYS; i++){
        mppts[i].voltage = 0;
    }
}

void DataManagement::zeroCurr(){
    for (int i = 0; i < NUM_ARRAYS; i++){
        mppts[i].current = 0;
    }
}

void DataManagement::read_ADC(){
    for (int i = 0; i < NUM_ARRAYS; i++) {
        mppts[i].voltage = getVoltIn(i);
        mppts[i].current = getCurrIn(i);
        mppts[i].power = mppts[i].voltage * mppts[i].current;
    }
}

//Gets the battery voltage
float DataManagement::getBattVolt(){
    return batteryVoltIn.read() * BV_SCALE;
}