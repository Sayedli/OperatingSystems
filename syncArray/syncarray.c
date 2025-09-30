// Author: Hassan Ali

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include "syncarray.h"

// ---------------------------------
// Synchronized Array Data Structure
// ---------------------------------

#define ARRAY_SIZE 50000
#define DEBUG 0

#include "syncarray.h"

void init(syncarray *c) {
    int i;
    for (i = 0; i < ARRAY_SIZE; i++) {
        c->value[i] = 0;
    }
    c->index = 0;
	// Initializing mutex
    pthread_mutex_init(&c->lock, NULL); 
}

void destroy(syncarray *c) {
	// Destroying mutex
    pthread_mutex_destroy(&c->lock); 
}

int put(syncarray *c, long val) {
	// Locking mutex
    pthread_mutex_lock(&c->lock); 
    int store_index = c->index;
    if (store_index < ARRAY_SIZE) {
        c->value[store_index] = val;
        c->index++;
	// Unlocking mutex
        pthread_mutex_unlock(&c->lock); 
        return store_index;
    }
	// Unlocking mutex
    pthread_mutex_unlock(&c->lock); 
    return -1;
}

long sum(syncarray *c) {
	// Locking mutex
    pthread_mutex_lock(&c->lock); 
    long sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += c->value[i];
    }
	// Unlocking mutex
    pthread_mutex_unlock(&c->lock); 
    return sum;
}

long avg(syncarray *c) {
    long total = sum(c); 
    return total / ARRAY_SIZE;
}
