#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define n 3 // Number of threads

void *func1(void *arg) {
    for (int i=0; i < 10; i++) {
        printf("Hello from func1\n");
        sleep(1);
    }
    return NULL;
}

void *func2(void *arg) {
    for (int i=0; i < 5; i++) {
        printf("Hello from func2\n");
        sleep(2);
    }
    return NULL;
}

void *func3(void *arg) {
    for (int i=0; i < 2; i++) {
        printf("Hello from func3\n");
        sleep(5);
    }
    return NULL;
}

// MAIN
int main() {
    // Set up threads
    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, (void *) func1, NULL);
    pthread_create(&thread2, NULL, (void *) func2, NULL);
    pthread_create(&thread3, NULL, (void *) func3, NULL);

    // Signal experiments
    printf("Before SIGINT signals\n");
    //kill(getpid(), SIGINT);
    //pthread_kill(thread2, SIGINT);
    printf("After SIGINT signals\n");

    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    return 0;
}