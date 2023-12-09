#include "mbed.h"
#include "const.h"
#include "FastPWM.h"
#include "mppt.h"
#include "pid.h"

constexpr float I_SCALE = 5*3.33;
constexpr float V_SCALE = (103.3/3.3)*3.33;
constexpr float BV_SCALE = 3.325 * 101;

class DataManagement{
    public:
        //Increments voltage with new voltage reading
        void incVolt();
        //Sets previous voltage to voltage
        void updatePrevVolt();

       //Increments voltage with new voltage reading
        void incCurr();
        //Sets previous current to current
        void updatePrevCurr();
        //Sets previous power to power
        void updatePrevPow();

        //Get stored voltage
        float getVolt(int array_num);
        //Get accumulated voltage to average
        float getVoltAcc(int array_num);
        //Get stored previous voltage
        float getPrevVolt(int array_num);

        //Get stored current
        float getCurr(int array_num);
        //Get acculated current to average
        float getCurrAcc(int array_num);
        //Get stored previous current
        float getPrevCurr(int array_num);

        //Get stored power
        float getPower(int array_num);
        //Get store previous current
        float getPrevPower(int array_num);

        //Clears values stored in mppt voltage and current
        void zeroVolt();
        void zeroCurr();

        //Reads new voltage and current values, calcs and saves power
        void read_ADC();

        //Calculates power using previous voltage/current values
        //float getPower(int arrayIdx);

        //Gets the battery voltage
        float getBattVolt();

        FastPWM outputs[NUM_ARRAYS] = {FastPWM(MOSFET_1), FastPWM(MOSFET_2), FastPWM(MOSFET_3)};

        //Array of struct for PID constants
        PID pids[NUM_ARRAYS];

        //Array of struct for volt/curr data
        MPPT mppts[NUM_ARRAYS];

        //Store battery voltage
        float battVolt;

        DataManagement();

    private:
        struct Input{
            AnalogIn vIn;
            AnalogIn iIn;
        };

        //Gets new voltage from specificied solar array
        float getVoltIn(int arrayIdx);

        //Gets new current of specificied solar array
        float getCurrIn(int arrayIdx);

        AnalogIn batteryVoltIn = AnalogIn(BATTERY_VOLT_IN);
        
        //Voltage and Current In for each solar array section
        Input inputs[NUM_ARRAYS] = {{AnalogIn(PinName(VOLT_IN_1)), AnalogIn(PinName(CURR_IN_1))},
                            {AnalogIn(PinName(VOLT_IN_2)), AnalogIn(PinName(CURR_IN_2))},
                            {AnalogIn(PinName(VOLT_IN_3)), AnalogIn(PinName(CURR_IN_3))}};

};