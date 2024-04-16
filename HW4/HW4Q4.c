#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define n 3 // Number of threads

// Signal handler for SIGINT
void sigHandler(int sig, siginfo_t *info, void *extra) {
    printf("Caught signal %d in %ld\n", sig, pthread_self());
}

// Sets up signal handler with sigaction
void setSigHandler(void) {
    struct sigaction action;
    action.sa_flags = SA_SIGINFO; 
    action.sa_sigaction = sigHandler;

    if (sigaction(SIGINT, &action, NULL) == -1) { 
        perror("sigusr: sigaction");
        _exit(1);
    }
}

// Signal masking
void maskSig(void) {
    sigset_t sigset; 
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);
}

// Thread 1 function
// Signal handling: default
void *func1(void *arg) {
    signal(SIGINT, SIG_DFL);

    for (int i=0; i < 5; i++) {
        printf("Hello from func1\n");
        sleep(2);
    }
    return NULL;
}

// Thread 2 function
// Signal handling: ignore (sigmask)
void *func2(void *arg) {
    maskSig();

    for (int i=0; i < 5; i++) {
        printf("Hello from func2\n");
        sleep(2);
    }
    return NULL;
}

// Thread 3 function
// Signal handling: custom handler
void *func3(void *arg) {
    setSigHandler();

    for (int i=0; i < 5; i++) {
        printf("Hello from func3\n");
        sleep(2);
    }
    return NULL;
}

// MAIN
int main() {
    pthread_t thread1, thread2, thread3;

    // Create threads
    pthread_create(&thread1, NULL, (void *) func1, NULL);
    pthread_create(&thread2, NULL, (void *) func2, NULL);
    pthread_create(&thread3, NULL, (void *) func3, NULL);

    sleep(1);

    // Signal experiments
    printf("Before SIGINT signals\n");
    //kill(getpid(), SIGINT);
    //pthread_kill(thread1, SIGINT);
    //pthread_kill(thread2, SIGINT);
    pthread_kill(thread3, SIGINT);
    printf("After SIGINT signals\n");

    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_exit(NULL);
    return 0;
}