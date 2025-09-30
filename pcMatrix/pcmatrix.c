// Author: Hassan Ali
// EXTRA CREDIT FEATURES: EC1, EC2, EC3 implemented

/*
 *  pcmatrix module
 *  Primary module providing control flow for the pcMatrix program
 *
 *  Producer consumer bounded buffer program to produce random matrices in parallel
 *  and consume them while searching for valid pairs for matrix multiplication.
 *  Matrix multiplication requires the first matrix column count equal the
 *  second matrix row count.
 *
 *  A matrix is consumed from the bounded buffer.  Then matrices are consumed
 *  from the bounded buffer, one at a time, until an eligible matrix for multiplication
 *  is found.
 *
 *  Totals are tracked using the ProdConsStats Struct for:
 *  - the total number of matrices multiplied (multtotal from consumer threads)
 *  - the total number of matrices produced (matrixtotal from producer threads)
 *  - the total number of matrices consumed (matrixtotal from consumer threads)
 *  - the sum of all elements of all matrices produced and consumed (sumtotal from producer and consumer threads)
 *
 *  Correct programs will produce and consume the same number of matrices, and
 *  report the same sum for all matrix elements produced and consumed.
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include "matrix.h"
#include "counter.h"
#include "prodcons.h"
#include "pcmatrix.h"

int BOUNDED_BUFFER_SIZE;
int NUMBER_OF_MATRICES;
int MATRIX_MODE;

int main(int argc, char *argv[])
{
    // Process command line arguments
    int numw = NUMWORK;
    if (argc == 1)
    {
        BOUNDED_BUFFER_SIZE = MAX;
        NUMBER_OF_MATRICES = LOOPS;
        MATRIX_MODE = DEFAULT_MATRIX_MODE;
        printf("USING DEFAULTS: worker_threads=%d bounded_buffer_size=%d matrices=%d matrix_mode=%d\n", numw, BOUNDED_BUFFER_SIZE, NUMBER_OF_MATRICES, MATRIX_MODE);
    }
    else
    {
        if (argc == 2)
        {
            numw = atoi(argv[1]);
            BOUNDED_BUFFER_SIZE = MAX;
            NUMBER_OF_MATRICES = LOOPS;
            MATRIX_MODE = DEFAULT_MATRIX_MODE;
        }
        if (argc == 3)
        {
            numw = atoi(argv[1]);
            BOUNDED_BUFFER_SIZE = atoi(argv[2]);
            NUMBER_OF_MATRICES = LOOPS;
            MATRIX_MODE = DEFAULT_MATRIX_MODE;
        }
        if (argc == 4)
        {
            numw = atoi(argv[1]);
            BOUNDED_BUFFER_SIZE = atoi(argv[2]);
            NUMBER_OF_MATRICES = atoi(argv[3]);
            MATRIX_MODE = DEFAULT_MATRIX_MODE;
        }
        if (argc == 5)
        {
            numw = atoi(argv[1]);
            BOUNDED_BUFFER_SIZE = atoi(argv[2]);
            NUMBER_OF_MATRICES = atoi(argv[3]);
            MATRIX_MODE = atoi(argv[4]);
        }
        printf("USING: worker_threads=%d bounded_buffer_size=%d matrices=%d matrix_mode=%d\n", numw, BOUNDED_BUFFER_SIZE, NUMBER_OF_MATRICES, MATRIX_MODE);
    }

    time_t t;
    // Seed the random number generator with the system time
    srand((unsigned) time(&t));
    initialize_buffer();

    printf("Producing %d matrices in mode %d.\n", NUMBER_OF_MATRICES, MATRIX_MODE);
    printf("Using a shared buffer of size=%d\n", BOUNDED_BUFFER_SIZE);
    printf("With %d producer and consumer thread(s).\n", numw);
    printf("\n");

    pthread_t producers[numw];
    pthread_t consumers[numw];
    ProdConsStats prods_stats[numw];
    ProdConsStats cons_stats[numw];

    // Create producer and consumer threads
    for (int i = 0; i < numw; i++)
    {
        pthread_create(&producers[i], NULL, prod_worker, &(prods_stats[i]));
        pthread_create(&consumers[i], NULL, cons_worker, &(cons_stats[i]));
    }

    // Aggregate total numbers for main thread output
    int prs = 0;      // total sum of elements for matrices produced
    int cos = 0;      // total sum of elements for matrices consumed
    int prodtot = 0;  // total number of matrices produced
    int constot = 0;  // total number of matrices consumed
    int consmul = 0;  // total number of multiplications

    // Wait for all threads to complete and aggregate results
    for (int i = 0; i < numw; i++)
    {
        pthread_join(producers[i], NULL);
        prs += prods_stats[i].sumtotal;
        prodtot += prods_stats[i].matrixtotal;
        pthread_join(consumers[i], NULL);
        cos += cons_stats[i].sumtotal;
        constot += cons_stats[i].matrixtotal;
        consmul += cons_stats[i].multtotal;
    }

    // Print the final results
    printf("Sum of Matrix elements --> Produced=%d = Consumed=%d\n", prs, cos);
    printf("Matrices produced=%d consumed=%d multiplied=%d\n", prodtot, constot, consmul);

    return 0;
}
