#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
using namespace std;

// Sieve of Eratosphenes
void *childThread(void *arg) {
    int n = *((int *) arg);
    int lim = (int) sqrt(n);
    return NULL;
}

// MAIN
int main(int argc, char *argv[]) {
    vector<int> primes;
    while (1) {
        // Get user input
        int n;
        printf("Enter the limit for the number of primes: \n");
        scanf("%d", &n);

        // Check input
        if (n <= 1) {
            printf("Invalid input\n");
            return 1;
        }
    }


    //pthread_create(&childThread, NULL, (void *) &printHello, NULL);
    return 0;
}