#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// MAIN
int main(int argc, char *argv[]) {
    FILE *fptr;

    // Initialize random generator
    srand(time(NULL));

    // Incorrect number of arguments
    if (argc != 3) {
        printf("Incorrect number of arguments (%d), should be 3!\n", argc-1);
        exit(1);
    }

    // Get values of program arguments
    int L = atoi(argv[1]);
    int H = atoi(argv[2]);

    // Check L bounds
    if (L < 10000) {
        printf("Invalid L value (%d). L needs to be at least 10,000!\n", L);
        exit(1);
    }

    // Check H bounds
    if ((H < 30) || (H > 60)) {
        printf("Invalid H value (%d). H needs to be between 30 and 60!\n", H);
        exit(1);
    }

    // Attempt to create the file
    fptr = fopen("generatedList.txt", "w");
    if (!fptr) {
        // TODO: Error on file create
        exit(1);
    }

    // Generate random values
    int randVals[L];
    for (int i = 0; i < L; i++) {
        randVals[i] = rand();
    }

    // Generate random keys
    int keys[H];
    int min = 0;
    int avg = 0;
    for (int i = 0; i < H; i++) {
        keys[i] = -((rand() % 59) + 1);
        avg += keys[i];
        if (keys[i] < min) {
            min = keys[i];
        }
        //printf("Key #%d: %d\n", i+1, keys[i]);
    }
    avg = -1 * (avg / H); // Calculate average
    int max = -1 * min; // Invert min to find max
    printf("Key Max: %d\n", max);
    printf("Key Avg: %d\n", avg);

    // Generate random positions of keys and insert keys
    int keyPos[H];
    for (int i = 0; i < H; i++) {
        keyPos[i] = rand() % L; 
        //printf("Key #%d at position %d\n", i+1, keyPos[i]);
        randVals[keyPos[i]] = keys[i];
        //printf("Key #%d value at position %d: %d\n", i+1, keyPos[i], randVals[keyPos[i]]);
    }

    // Write randVals to file
    for (int i = 0; i < L; i++) {
        fprintf(fptr, "%d\n", randVals[i]);
    }

    // Close the file
    fclose(fptr);

    return 0;
}