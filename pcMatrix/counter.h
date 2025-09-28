/*
 *  counter header
 *  Function prototypes, data, and constants for synchronized counter module
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 */

// SYNCHRONIZED COUNTER

#ifndef COUNTER_H
#define COUNTER_H

#include <pthread.h>

typedef struct __counter_t {
    int value;
    pthread_mutex_t lock;
} counter_t;

typedef struct __counters_t {
    int value;
    pthread_mutex_t lock;
} counters_t;

void init_cnt(counter_t *c);
void increment_cnt(counter_t *c);
void decrement_cnt(counter_t *c);
int get_cnt(counter_t *c);

#endif // COUNTER_H
