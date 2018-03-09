#include "zuul.h"

#define MICROS_IN_S 100000000

enum text_color {
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

static const char *get_color(enum text_color c) {
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

void printc(enum text_color c, const char *format, ...) {
    va_list args;
    va_start(args, format);

    const char *color = get_color(c);
    const char *rst = get_color(NORMAL);

    printf("%s", color);
    vprintf(format, args);
    printf("%s", rst);
    va_end(args);
}

void section(const char *name) {
    printc(BLUE, "Testing ");
    printc(BLUE_B, name);
    printc(BLUE, ":\n");
}

void assert_section(const char *name, struct t_result *results) {
    printc(BLUE, "    » ");
    printc(BLUE_B, "%s", name);
    printc(BLUE, " | ");
    enum text_color status = (results->failed > 0) ? RED_B : GREEN_B;
    size_t total = results->failed + results->succeeded;
    printc(status, "PASSED %zu/%zu", results->succeeded, total);
    printc(BLUE, " | TOOK %Lf\n", results->total / MICROS_IN_S);
}

long double elapsed_micros(struct timeval start, struct timeval end) {
    long double start_micros, end_micros;
    start_micros = (long double) (start.tv_sec * MICROS_IN_S) + (long double) (start.tv_usec);
    end_micros = (long double) (end.tv_sec * MICROS_IN_S) + (long double) (end.tv_usec);
    return end_micros - start_micros;
}


struct test_time {
    long double max;
    long double min;
    long double avg;
    long double total;
};

struct test_time compute_time(const long double *times, size_t len) {
    struct test_time t = {.total = 0, .avg = 0, .min = times[0], .max = 0};
    for (size_t i = 0; i < len; ++i) {
        if (times[i] > t.max) t.max = times[i];
        if (times[i] < t.min) t.min = times[i];
        t.total += times[i];
    }
    t.avg = t.total / len;
    return t;
}

void print_timing(struct test_time *t) {
    printc(GREEN, "MIN: %Lf", t->min / MICROS_IN_S);
    printc(BLUE, " | ");
    printc(MAGENTA, "MAX: %Lf", t->max / MICROS_IN_S);
    printc(BLUE, " | ");
    printc(CYAN, "AVG: %Lf", t->avg / MICROS_IN_S);
}

struct thread_data {
    const char *name;
    bool kill;
};

void *loading(void *arg) {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    struct thread_data *t = (struct thread_data *) (arg);

    //static const char load[4] = {'/', '-', '\\', '-'};
    static const char *load[] = {"▁", "▂", "▃", "▄", "▅", "▆", "▇", "█", "▇", "▆", "▅", "▄", "▃", "▁"};
    size_t len = sizeof(load) / sizeof(char *);
    size_t idx = 0;


    while (true) {
        if (t->kill) break;

        printc(YELLOW, "    %s", load[idx % len]);
        printc(BLUE, " TESTING | ");
        printc(YELLOW_B, "%s\r", t->name);

        ++idx;
        fflush(stdout);
        usleep(200000);
    }

    return NULL;
}

bool assert(const char *name, bool (*test_fn)(), struct t_result *results, size_t runs) {
    bool result = true;
    struct thread_data load = {.name=name, .kill=false};
    pthread_t load_thread;
    long double *buf = malloc(runs * sizeof(long double));

    pthread_create(&load_thread, NULL, loading, &load);
    for(size_t i = 0; i < runs; ++i){
        struct timeval start, end;

        gettimeofday(&start, 0);
        result &= (*test_fn)();
        gettimeofday(&end, 0);

        // We could use a rolling average, but precision is crucial here
        buf[i] = elapsed_micros(start, end);
    }
    pthread_cancel(load_thread);

    struct test_time t = compute_time(buf, runs);
    free(buf);
    results->total += t.total;

    printf("\r");
    if (result) {
        printc(GREEN_B, "    ✓ ");
        printc(GREEN, "SUCCESS");
        ++(results->succeeded);
    } else {
        printc(RED_B, "    ✕ ");
        printc(RED, "FAIL   ");
        ++(results->failed);
    }
    printc(BLUE, " | ");
    print_timing(&t);
    printc(BLUE, " | ");
    printc(YELLOW_B, "%s\n", name);
    return result;
}