#ifndef ZUUL_THREADPOOL_H
#define ZUUL_THREADPOOL_H

#include "fifo.h"
#include "zuul.h"
#include <pthread.h>

typedef struct threadpool{
    struct FIFO *tasks;
    pthread_t *workers;
    size_t worker_count;

    bool result;
    long double *timing;

    void (*add)(struct threadpool *, struct test *t);
    void (*consume)(struct threadpool *);
    void (*kill)(struct threadpool **);
} tpool;

tpool  *threadpool_init(size_t worker_count);
void threadpool_add(tpool *tp, struct test *t);
void threadpool_consume(tpool *tp);
void threadpool_kill(tpool **tp);

size_t get_cores();
#endif //ZUUL_THREADPOOL_H
