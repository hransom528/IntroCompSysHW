#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
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
    for (int i = 2; i < n; i++) {
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
    fibStruct fs;
    fs.n = n;
    fs.fib = (int *) malloc(sizeof(int) * n);
    fs.done = (bool *) calloc(n, sizeof(bool));
    pthread_create(&childThread, NULL, &childThreadFunc, (void *) &fs);
    
    // Wait for child thread to finish executing
    int i = 0;
    while (fs.done[n-1] == false) {
        if (fs.done[i]) {
            printf("%d ", fs.fib[i]);
            i++;
        }
    }
    pthread_join(childThread, NULL);
    printf("\n");
    
    return 0;
}