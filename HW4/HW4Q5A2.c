#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

typedef struct threadStruct {
    int n;
    int *fib;
    bool *done;
} fibStruct;

// Child thread function
void *childThreadFunc(void *arg) {
    fibStruct *fs = (fibStruct *) arg;
    
    // Calculate Fib series
    fs->fib[0] = 1;
    fs->fib[1] = 1;
    fs->done[0] = true;
    fs->done[1] = true;
    for (int i = 2; i < fs->n; i++) {
        fs->fib[i] = fs->fib[i-1] + fs->fib[i-2];
        fs->done[i] = true;
    }
    return NULL;
}

// MAIN
int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    pthread_t childThread;

    // Check input
    if (n <= 0) {
        printf("Invalid input\n");
        return 1;
    }

    // Create child thread
    fibStruct *fs = malloc(sizeof(fibStruct));
    fs->n = n;
    fs->fib = (int *) malloc(sizeof(int) * n);
    fs->done = (bool *) calloc(n, sizeof(bool));
    pthread_create(&childThread, NULL, &childThreadFunc, (void *) fs);

    // Grab current items
    int index = 0;
    int results[n];
    while ((index != n) || (!fs->done[n-1])) {
        for (int i = index; i < n; i++) {
            if (fs->done[i]) {
                printf("%d ", fs->fib[i]);
                results[i] = fs->fib[i];
                index++;
            }
        }
        sleep(0.0001);
    }

    // Print out results array
    /*
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", results[i]);
    }*/

    pthread_join(childThread, NULL);
    printf("\n");
    
    return 0;
}