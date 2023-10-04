#include "terminal.h"

int main(void) {

    int len;
    char buf[BUFFER_SIZE] = {};
    if ((len = read(buf)) < 0) exit(1);
    printf("Length of string %s: %d\n", buf, len);
}