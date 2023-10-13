#ifndef TERMINAL_H
#define TERMINAL_H

#include "mbed.h"
#include <string.h>

#define BUFFER_SIZE 16

int read(char* dest, int len, bool verbose = false);

#endif