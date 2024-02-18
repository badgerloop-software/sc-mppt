#ifndef __CAN_MPPT_H__
#define __CAN_MPPT_H__

#include "mbed.h"
#include "canmanager.h"
#include "const.h"
#include "IOManagement.h"

class CANMPPT : public CANManager {
    public:
        CANMPPT(PinName rd, PinName td, int frequency = DEFAULT_CAN_FREQ);
        void readHandler(int messageID, SharedPtr<unsigned char> data, int length);
        void sendMPPTData();
};

#endif // __CAN_MPPT_H__