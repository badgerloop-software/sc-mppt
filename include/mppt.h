#ifndef __MPPT_H__
#define __MPPT_H__


#include "mbed.h"
#include "const.h"
#include "IOManagement.h"


// Sets up running of MPPT algorithm at specified rate
void initMPPT(std::chrono::microseconds updatePeriod);


#endif // __MPPT_H__