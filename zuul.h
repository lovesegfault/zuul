#ifndef ZUUL_ZUUL_H
#define ZUUL_ZUUL_H

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>


typedef bool (*test_fn)();

struct t_result{
    size_t succeeded;
    size_t failed;
    long double total;
};

void section(const char *name);
void assert_section(const char *name, struct t_result *results);
bool assert(const char *name, test_fn test, struct t_result *results, size_t runs);

#endif //ZUUL_ZUUL_H
