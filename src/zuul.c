#include "zuul.h"

inline struct test *test_init(const char *name, bool (*test_fn)(), size_t runs) {
    struct test *t = calloc(1, sizeof(struct test));
    t->name = name;
    t->test_fn = test_fn;
    t->runs = runs;

    t->assert = test_assert;
    return t;
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

inline bool test_assert(struct test *t) {
    bool result = true;
    struct thread_data load = {.name=t->name, .kill=false};
    return result;
}

struct section section_init(const char *name) {
    struct section s = {};

    s.name = name;
    s.tests = fifo_init();
    s.add_test = section_add_test;
    s.assert = section_assert;
    s.kill = section_kill;

    return s;
}

void section_kill(struct section *s) { s->tests->free(&s->tests, true); }

void section_add_test(struct section *s, const char *name, bool (*test_fn)(),
                      size_t runs) {
    s->tests->enqueue(s->tests, test_init(name, test_fn, runs));
}

void section_assert(struct section *s) {
    printc(BLUE, "Testing ");
    printc(BLUE_B, s->name);
    printc(BLUE, ":\n");
}