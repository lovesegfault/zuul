#ifndef ZUUL_PRINTC_H
#define ZUUL_PRINTC_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

enum tcolor {
    RED,
    RED_B,
    GREEN,
    GREEN_B,
    YELLOW,
    YELLOW_B,
    BLUE,
    BLUE_B,
    MAGENTA,
    MAGENTA_B,
    CYAN,
    CYAN_B,
    NORMAL,
};

void printc(enum tcolor c, const char *format, ...);

#endif  // ZUUL_PRINTC_H
