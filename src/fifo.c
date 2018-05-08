#include "fifo.h"

/*
 * Initializes a FIFO queue
 */
fifo_t *fifo_init() {
    fifo_t *queue = calloc(1, sizeof(fifo_t));  // Allocate struct
    queue->last = NULL;
    queue->first = NULL;
    queue->mutex = calloc(1, sizeof(pthread_mutex_t));  // Allocate mutex
    pthread_mutex_init(queue->mutex,
                       NULL);  // Initialize the mutex, no attributes needed
    queue->count = 0;          // To allow for O(1) counting

    // Function pointers
    queue->count_mutex = fifo_count;
    queue->enqueue = fifo_enqueue;
    queue->dequeue = fifo_dequeue;
    queue->free = fifo_free;

    return queue;
}

/*
 * Enqueues into FIFO
 */
void fifo_enqueue(fifo_t *queue, struct test *data) {
    pthread_mutex_lock(queue->mutex);  // Lock
    // Allocate the node
    struct list_node *new = calloc(1, sizeof(struct list_node));
    new->data = data;  // Link data

    // If we are enqueuing on an empty list, set first and last to be the
    // singleton node
    if (queue->first == NULL) {
        queue->first = queue->last = new;
        ++queue->count;
        pthread_mutex_unlock(queue->mutex);
        return;
    }

    // Attach node
    queue->last->next = new;
    new->prev = queue->last;

    queue->last = new;
    ++queue->count;  // Increment element count

    pthread_mutex_unlock(queue->mutex);  // Unlock
}

/*
 * Dequeue from FIFO
 */
struct test *fifo_dequeue(fifo_t *queue) {
    pthread_mutex_lock(queue->mutex);  // Lock
    // Dequeueing on an empty FIFO returns NULL
    if (queue->first == NULL) {
        pthread_mutex_unlock(queue->mutex);
        return NULL;
    }

    // Detach node
    struct list_node *first = queue->first;
    queue->first = first->next;
    first->prev = NULL;

    // Save data pointer, free node
    struct test *data = first->data;
    free(first);

    --queue->count;                      // Decrement element count
    pthread_mutex_unlock(queue->mutex);  // Unlock

    return data;
}

/*
 * Free FIFO
 */
void fifo_free(fifo_t **queue, bool free_data) {
    pthread_mutex_lock((*queue)->mutex);  // Lock

    // Iterate over FIFO, freeing nodes
    struct list_node *index;
    while ((index = (*queue)->first) != NULL) {
        (*queue)->first = (*queue)->first->next;
        // Optional data freeing
        if (free_data) {
            free(index->data);
            index->data = NULL;
        }
        free(index);
    }
    // Clean mutex
    pthread_mutex_unlock((*queue)->mutex);
    pthread_mutex_destroy(((*queue)->mutex));
    free((*queue)->mutex);
    // Free structure
    free(*queue);
    *queue = NULL;
}

size_t fifo_count(fifo_t *queue) {
    pthread_mutex_lock(queue->mutex);    // Lock
    size_t res = queue->count;           // Save count
    pthread_mutex_unlock(queue->mutex);  // Unlock
    return res;
}