#include "terminal.h"

int main(void) {
    Terminal term;

    int len;
    char buf[BUFFER_SIZE] = {};
    if ((len = term.read(buf)) < 0) exit(1);
    printf("Length of string %s: %d\n", buf, len);
}