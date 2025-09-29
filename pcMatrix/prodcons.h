/*
 *  prodcons header
 *  Function prototypes, data, and constants for producer/consumer module
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 */

#ifndef PRODCONS_H
#define PRODCONS_H

#include "matrix.h"
#include "counter.h"

// Bounded buffer for matrices
extern Matrix **bigmatrix;

// Data structure to track matrix production / consumption stats
// sumtotal - total of all elements produced or consumed
// multtotal - total number of matrices multiplied
// matrixtotal - total number of matrices produced or consumed
typedef struct prodcons {
    int sumtotal;
    int multtotal;
    int matrixtotal;
} ProdConsStats;

// PRODUCER-CONSUMER thread method function prototypes
void *prod_worker(void *arg);
void *cons_worker(void *arg);

// Routines to add and remove matrices from the bounded buffer
int put(Matrix *value);
Matrix *get();

// Initialize the bounded buffer
void initialize_buffer();

#endif // PRODCONS_H
