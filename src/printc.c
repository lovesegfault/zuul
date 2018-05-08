#include "printc.h"

static inline const char *get_color(enum tcolor c) {
    static char *ansi = NULL;
    switch (c) {
        case RED:
            ansi = "\033[31m";
            break;
        case RED_B:
            ansi = "\033[1m\033[31m";
            break;
        case GREEN:
            ansi = "\033[32m";
            break;
        case GREEN_B:
            ansi = "\033[1m\033[32m";
            break;
        case YELLOW:
            ansi = "\x1b[33m";
            break;
        case YELLOW_B:
            ansi = "\033[1m\x1b[33m";
            break;
        case BLUE:
            ansi = "\x1b[34m";
            break;
        case BLUE_B:
            ansi = "\033[1m\x1b[34m";
            break;
        case MAGENTA:
            ansi = "\x1b[35m";
            break;
        case MAGENTA_B:
            ansi = "\033[1m\x1b[35m";
            break;
        case CYAN:
            ansi = "\x1b[36m";
            break;
        case CYAN_B:
            ansi = "\033[1m\x1b[36m";
            break;
        case NORMAL:
            ansi = "\033[0m";
            break;
    }
    return ansi;
}

void printc(enum tcolor c, const char *format, ...) {
    va_list args;
    va_start(args, format);

    const char *color = get_color(c);
    const char *rst = get_color(NORMAL);

    printf("%s", color);
    vprintf(format, args);
    printf("%s", rst);
    va_end(args);
}