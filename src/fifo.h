#ifndef ZUUL_FIFO_H
#define ZUUL_FIFO_H

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "zuul.h"

struct list_node {
    struct list_node *prev;
    struct list_node *next;
    struct test *data;
};

typedef struct FIFO {
    struct list_node *first;
    struct list_node *last;

    size_t count;
    pthread_mutex_t *mutex;

    size_t (*count_mutex)(struct FIFO *);

    void (*enqueue)(struct FIFO *, struct test *);

    struct test *(*dequeue)(struct FIFO *);

    void (*free)(struct FIFO **, bool);
} fifo_t;

fifo_t *fifo_init();

void fifo_enqueue(fifo_t *queue, struct test *data);

struct test *fifo_dequeue(fifo_t *queue);

void fifo_free(fifo_t **queue, bool free_data);

size_t fifo_count(fifo_t *queue);

#endif  // ZUUL_FIFO_H
