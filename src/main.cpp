#include "terminal.h"

int main(void) {

    int len;
    char buf[BUFFER_SIZE] = {};
    if ((len = read(buf, BUFFER_SIZE)) < 0) printf("Error\n");
    printf("Length of string %s: %d\n", buf, len);

    char smallbuf[1] = {};
    if ((len = read(smallbuf, 1, true)) < 0) printf("Error\n");
    printf("Length of string %s: %d\n", smallbuf, len);
}