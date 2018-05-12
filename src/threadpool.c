#include "threadpool.h"

tpool *threadpool_init(size_t worker_count) {
  tpool *tp = calloc(1, sizeof(tpool));
  tp->tasks = fifo_init();
  tp->workers = calloc(worker_count, sizeof(pthread_t));
  if (tp->workers == NULL)
    return NULL;
  tp->worker_count = worker_count;

  tp->result = true;

  tp->add = threadpool_add;
  tp->consume = threadpool_consume;
  tp->kill = threadpool_kill;

  return tp;
}

void threadpool_add(tpool *tp, struct test *t) {
  tp->tasks->enqueue(tp->tasks, t);
}

struct consume_bus {
  fifo_t *tasks;
  bool done;
  bool result;

};

static void *consume_loop(void *arg) {
  struct consume_bus *bus = (struct consume_bus *)(arg);
  fifo_t *tasks = bus->tasks;
  bool result = true;
  struct test *t;
  while ((t = tasks->dequeue(tasks)) != NULL) {
    result &= t->test_fn();
  }
  bus->done = true;
  return NULL;
}

void threadpool_consume(tpool *tp) {
  struct consume_bus *buses =
      calloc(tp->worker_count, sizeof(struct consume_bus));
  tp->timing = calloc(tp->tasks->count, sizeof(long double));

  for (size_t i = 0; i < tp->worker_count; ++i) {
    buses[i] = (struct consume_bus){tp->tasks, false, true};
    pthread_create(&(tp->workers[i]), NULL, consume_loop, &(buses[i]));
  }

  size_t done = 0;
  while (done < tp->worker_count) {
    for (size_t i = 0; i < tp->worker_count; ++i) {
      if ((buses[i]).done == true) {
          ++done;
          tp->result &= buses[i].result;
      }
    }
  }

  for (size_t i = 0; i < tp->worker_count; ++i)
    pthread_join((tp->workers)[i], NULL);
  free(buses);
}
void threadpool_kill(tpool **tp) {
  (*tp)->tasks->free(&((*tp)->tasks), true);
  free((*tp)->workers);
  free(*tp);
  *tp = NULL;
}

#ifdef _MACOS
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <sys/sysinfo.h>
#endif
size_t get_cores() {
#ifdef MACOS
  int nm[2];
  size_t len = 4;
  uint32_t count;

  nm[0] = CTL_HW;
  nm[1] = HW_AVAILCPU;
  sysctl(nm, 2, &count, &len, NULL, 0);

  if (count < 1) {
    nm[1] = HW_NCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);
    if (count < 1) {
      count = 1;
    }
  }
  return (size_t)count;
#else
  return (size_t)(get_nprocs());
#endif
}
