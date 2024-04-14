#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Child thread function
void *childThread(void *arg) {
    int n = *((int *) arg);
    int *fib = malloc(sizeof(int) * n);

    // Calculate Fib series
    fib[0] = 1;
    fib[1] = 1;
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i-1] + fib[i-2];
    }

    return (void *)fib;
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
    int *result;
    pthread_create(&childThread, NULL, (void *) &printHello, (void *) &n);
    
    // Wait for child thread to finish executing

    pthread_join(childThread, (void *) &result);

    // TODO: Collect and output results
    

    return 0;
}