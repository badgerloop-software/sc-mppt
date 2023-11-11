#include "mbed.h"
#include "const.h"
#include "FastPWM.h"
constexpr float I_SCALE = 5*3.33;
constexpr float V_SCALE = (103.3/3.3)*3.33;
constexpr float BV_SCALE = 3.325 * 101;

class DataManagement{
    public:
        //Gets new voltage from specificied solar array
        float getVoltIn(int arrayIdx);

        //Gets new current of specificied solar array
        float getCurrIn(int arrayIdx);

        //Calculates power using previous voltage/current values
        //float getPower(int arrayIdx);

        //Gets the battery voltage
        float getBattVolt();

         struct Data{
            //Most Power Point Voltage input (Voltage input for max power output)
            float targetVolt;
            float dutyCycle;
            //Most Power Point Voltage Duty (optimal duty cycle to charge battery)
            float mppDutyCycle;
        };

        Data data[NUM_ARRAYS];

        FastPWM outputs[NUM_ARRAYS] = {FastPWM(MOSFET_1), FastPWM(MOSFET_2), FastPWM(MOSFET_3)};

        DataManagement();

    private:
        struct Input{
            AnalogIn vIn;
            AnalogIn iIn;
        };

        AnalogIn batteryVoltIn = AnalogIn(BATTERY_VOLT_IN);

        //Voltage and Current In for each solar array section
        Input inputs[NUM_ARRAYS] = {{AnalogIn(PA_6), AnalogIn(PA_7)},
                            {AnalogIn(PA_4), AnalogIn(PA_5)},
                            {AnalogIn(PA_1), AnalogIn(PA_3)}};

};