// Author: Hassan Ali

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "syncarray.h"

void *worker(void *arg) {
    syncarray *sa = (syncarray *) arg;
    for (long counter = 1; counter <= 25000; counter++) {
        if (put(sa, counter) == -1) {
            fprintf(stderr, "Array index out of bounds\n");
            break;
        }
    }
    return NULL;
}

int main() {
    pthread_t p1, p2;
    syncarray sa;

    init(&sa);
    printf("Must run this program on a computer with at least 2 CPUs.\n\n");
    printf("The initial array sum is %ld\n", sum(&sa));

    pthread_create(&p1, NULL, worker, &sa);
    pthread_create(&p2, NULL, worker, &sa);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("The final array sum is %ld\n", sum(&sa));
    printf("The avg array element is %ld\n", avg(&sa));

    destroy(&sa);
    return 0;
}
