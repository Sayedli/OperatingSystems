// Author: Hassan Ali

#ifndef SYNCARRAY_H
#define SYNCARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>

#define ARRAY_SIZE 50000
#define DEBUG 0

typedef struct {
    long  value[ARRAY_SIZE];
    int  index;
	// To allow thread synchronization
    pthread_mutex_t lock;  
} syncarray;

void init(syncarray *c);
void destroy(syncarray *c);
int put(syncarray *c, long val);
long get(syncarray *c, int index);
long sum(syncarray *c);
long avg(syncarray *c);

#endif
