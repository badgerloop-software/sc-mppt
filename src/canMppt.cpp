#include "canMppt.h"


CANMPPT::CANMPPT(PinName rd, PinName td, int frequency) : CANManager(rd, td, frequency) {};

void CANMPPT::readHandler(int messageID, SharedPtr<unsigned char> data, int length) {

}

void CANMPPT::sendMPPTData() {
    this->sendMessage(0x400, (void*)&chargeMode, sizeof(ChargeMode));
    // For each array, offset by 3*i since sending 3 fields voltage, current, temperature
    for (int i = 0; i < NUM_ARRAYS; i++) {
        this->sendMessage(0x401 + 3*i, (void*)&(arrayData[i].voltage), sizeof(float));
        this->sendMessage(0x402 + 3*i, (void*)&(arrayData[i].current), sizeof(float));
        this->sendMessage(0x403 + 3*i, (void*)&(arrayData[i].voltage), sizeof(float)); // Placeholder until temp reading implemented
    }
}