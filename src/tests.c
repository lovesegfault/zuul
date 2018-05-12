#include "zuul.h"
#include "threadpool.h"
#include <unistd.h>
#include <stdlib.h>

#define DEBUG 1

bool test(){
    sleep(1);
    return true;
}

int main(){
    if(DEBUG) printf(">>> DEBUG: get_cores() -> %zu", get_cores());
    struct test *t = test_init("Test", test, 80);
    tpool *tp = threadpool_init(get_cores());
    for(size_t i = 0; i < t->runs; ++i){
        tp->add(tp, t);
    }

    tp->consume(tp);
    tp->kill(&tp);
}