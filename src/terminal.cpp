#include <string.h>
#include "mbed.h"

#define BUFFER_SIZE 16
const char N[2] {0xd, '\0'}; // newline character string

/**
* Read inputs from a BufferedSerial into a character buffer
* Terminates and returns when newline is detected
* 
* Parameters:
*   serial: buffer to read from (should be terminal)
*   buf: buffer to write to
* 
* Returns:
*   int: length of string written to buffer or -1 on error
**/
int readBuffer(BufferedSerial* serial, char* buf) {
    int strlen = 0; // length of string stored in character buffer
    int err = 0; // store value returned by BufferedSerial.read()
    bool overflow = false; // calling printf within loop is too slow

    while (1) {
        // check for buffer overflow before read
        if (strlen >= BUFFER_SIZE) { overflow = true; break; }

        // read in from serial
        if ((err = serial->read(buf + strlen, BUFFER_SIZE)) > 0) {
            strlen += err;
            buf[strlen] = '\0'; // convert to string

            // check for newline
            char* newline; // location of newline
            if ((newline = strstr(buf, N)) != NULL) {
                *newline = '\0'; // change \n to \0
                strlen = newline - buf;
                break;
            }
        } else if (err < 0) {
            if (strlen + err > BUFFER_SIZE) overflow = true;
            else printf("Error reading from buffer\n");
            return -1;
        }
    }

    if (overflow) { printf("Buffer overflow\n"); return -1; }
    return strlen;
}

int main(void) {
    BufferedSerial serial(USBTX, USBRX);
    char buf[BUFFER_SIZE] = {};

    int len;
    if ((len = readBuffer(&serial, buf)) < 0) exit(1);
    printf("Length of string %s: %d\n", buf, len);
}