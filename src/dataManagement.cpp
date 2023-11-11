#include "dataManagement.h"

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


//Gets the battery voltage
float DataManagement::getBattVolt(){
    return batteryVoltIn.read() * BV_SCALE;
}