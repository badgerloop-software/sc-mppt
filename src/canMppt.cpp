#include "canMppt.h"


CANMPPT::CANMPPT(PinName rd, PinName td, int frequency) : CANManager(rd, td, frequency) {};

void CANMPPT::readHandler(int messageID, SharedPtr<unsigned char> data, int length) {
    switch (messageID) {
        case 0x050:
            clearOVFaultReset(*data);
            break;
        case 0x051:
            setCapDischarge(*data);
            break;
        case 0x103:
            packChargeCurrentLimit = (float)(*(uint16_t*)data.get()) * CONST_CURR_SAFETY_MULT;
            setCurrentOut(packChargeCurrentLimit);
            break;
        default:
            break;
    }
}

void CANMPPT::sendMPPTData() {
    this->sendMessage(0x400, (void*)&chargeMode, sizeof(ChargeMode));
    // For each array, offset by 3*i since sending 3 fields voltage, current, temperature
    for (int i = 0; i < NUM_ARRAYS; i++) {
        this->sendMessage(0x401 + 3*i, (void*)&(arrayData[i].voltage), sizeof(float));
        this->sendMessage(0x402 + 3*i, (void*)&(arrayData[i].current), sizeof(float));
        this->sendMessage(0x403 + 3*i, (void*)&(arrayData[i].temp), sizeof(float)); // Placeholder until temp reading implemented
    }
}