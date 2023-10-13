#include "terminal.h"

/**
* Reads in from the terminal until newline is detected.
* Writes a null-terminated string into the provided destination buffer,
* not including the newline.
*
* Parameters:
*   dest (char*): pointer to destination buffer.
*   len (int): size of destination buffer.
*   verbose (bool): optional, defaults to false. if set to true,
*                   prints out descriptive error messages.
* 
* Returns:
*   int: length of string written to buffer or -1 on error.
**/
int read(char* dest, int len, bool verbose) {
    BufferedSerial *serial = new BufferedSerial(USBTX, USBRX);
    int strlen = 0; // length of string stored in character buffer
    int err = 0; // store value returned by BufferedSerial.read()
    bool overflow = false; // calling printf within loop is too slow

    while (1) {
        // check for buffer overflow before read
        if (strlen >= len) { overflow = true; break; }

        // read in from serial
        if ((err = serial->read(dest + strlen, len)) > 0) {
            strlen += err;
            dest[strlen] = '\0'; // convert to string

            // check for newline
            char* newline; // location of newline
            char n[2] {0xd, '\0'}; // newline character string
            if ((newline = strstr(dest, n)) != NULL) {
                *newline = '\0'; // change \n to \0
                strlen = newline - dest;
                break;
            }
        } else if (err < 0) {
            if (strlen + err > len) overflow = true;
            else if (verbose) printf("Error reading from buffer\n");
            return -1;
        }
    }

    if (overflow) { 
        if (verbose) printf("Buffer overflow\n");
        return -1;
    }
    return strlen;
}