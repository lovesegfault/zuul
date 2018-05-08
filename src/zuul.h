#ifndef ZUUL_H
#define ZUUL_H

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "fifo.h"
#include "printc.h"

struct test {
    const char *name;

    bool (*test_fn)();

    size_t runs;

    bool (*assert)(struct test *);
};

struct test *test_init(const char *name, bool (*test_fn)(), size_t runs);

bool test_assert(struct test *t);

struct section {
    const char *name;
    fifo_t *tests;

    size_t tests_succeeded;
    size_t tests_failed;
    long double total_time;

    void (*add_test)(struct section *, const char *, bool (*)(), size_t);

    void (*assert)(struct section *);

    void (*kill)(struct section *);
};

struct section section_init(const char *name);

void section_add_test(struct section *s, const char *name, bool (*test_fn)(),
                      size_t runs);

void section_assert(struct section *s);

void section_kill(struct section *s);

#endif // ZUUL_H
