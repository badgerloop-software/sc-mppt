#include "dataManagement.h"

DataManagement::DataManagement() {}

//Gets the voltage from specificied solar array
float DataManagement::getVoltIn(int array){
    return prevData[array].voltIn = inputs[array].vIn.read()*V_SCALE;
}

//Gets the current of specificied solar array
float DataManagement::getCurrIn(int array){
    // Declan's Equation : read()*3.3*0.943+0.0294;
    
    return prevData[array].currIn = inputs[array].iIn.read()*I_SCALE;
}

//Calculates the power of specificied solar array
float DataManagement::getPower(int array){
    return prevData[array].power = prevData[array].voltIn * prevData[array].currIn;
}

//Gets the battery voltage
float DataManagement::getBattVolt(){
    return batteryVoltIn.read() * BV_SCALE;
}