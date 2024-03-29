#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Generate file with list of L random integers and H random keys
int genRandomVals(int L, int H) {
// Attempt to create the file
    FILE *fptr = fopen("generatedList.txt", "w");
    if (!fptr) { // Error on file create
        return 1;
    }

    // Generate random values
    int randVals[L];
    for (int i = 0; i < L; i++) {
        randVals[i] = rand();
    }

    // Generate random keys
    int keys[H];
    for (int i = 0; i < H; i++) {
        keys[i] = -((rand() % 59) + 1);
        //printf("Key #%d: %d\n", i+1, keys[i]);
    }

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

    // Close the file and return
    fclose(fptr);
    return 0;
}

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

    // Generate randomized file
    int status = genRandomVals(L, H);
    if (!status) {
        printf("Random generation complete.\n");
    }
    else if (status == 1) {
        printf("Error! Random file could not be generated!\n");
        return 1;
    }

    // Open generated random file
    fptr = fopen("generatedList.txt", "r");
    if (!fptr) { // Error on file open
        printf("Error! Random generated file could not be opened!\n");
        return 2;
    }

    // TODO: Perform DFS or BFS
    printf("Beginning key search...\n");

    // Close the file and return
    fclose(fptr);
    return 0;
}