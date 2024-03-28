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
    if (argc != 4) {
        printf("Incorrect number of arguments (%d), should be 3!\n", argc-1);
        exit(1);
    }

    // Get values of program arguments
    int L = atoi(argv[1]);
    int H = atoi(argv[2]);
    int PN = atoi(argv[3]);

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
    fptr = fopen("generatedlist.txt", "w");
    if (!fptr) {
        // TODO: Error on file create
        exit(1);
    }

    // TODO: Generate random values
    int randVals[L];
    for (int i = 0; i < L; i++) {
        randVals[i] = rand();
    }

    // TODO: Generate random keys
    int keys[H];
    for (int i = 0; i < H; i++) {
        keys[i] = -((rand() % 59) + 1);
    }

    // TODO: Generate random positions of keys and insert keys
    int keyPos[H];
    for (int i = 0; i < H; i++) {
        keyPos[i] = rand() % L; 
        randVals[keyPos[i]] = keys[i];
    }

    // TODO: Write randVals to file


    // Close the file
    fclose(fptr);

    return 0;
}