#include "mbed.h"
#define NUM_ARRAYS 3
#define BATTERY_VOLT_IN PB_0
constexpr float I_SCALE = 5*3.33;
constexpr float V_SCALE = (103.3/3.3)*3.33;
constexpr float BV_SCALE = 3.325 * 101;

class DataManagement{
    public:
        //Gets the voltage from specificied solar array
        float getVoltIn(int array);

        //Gets the current of specificied solar array
        float getCurrIn(int array);

        //Calculates power using previous voltage/current values
        float getPower(int array);

        //Gets the battery voltage
        float getBattVolt();


        DataManagement();

    private:
        struct Input{
            AnalogIn vIn;
            AnalogIn iIn;
        };

        AnalogIn batteryVoltIn = AnalogIn(BATTERY_VOLT_IN);

        struct Data{
            //Most Power Point Voltage input (Voltage input for max power output)
            float voltIn;
            float currIn;
            float power;
            float targetVolt;
            float dutyCycle;
            //Most Power Point Voltage Duty (optimal duty cycle to charge battery)
            float mppDutyCycle;
        };

        Data prevData[NUM_ARRAYS];

        //Voltage and Current In for each solar array section
        Input inputs[NUM_ARRAYS] = {{AnalogIn(PA_6), AnalogIn(PA_7)},
                            {AnalogIn(PA_4), AnalogIn(PA_5)},
                            {AnalogIn(PA_1), AnalogIn(PA_3)}};

};