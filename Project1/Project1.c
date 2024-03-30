#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

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

    // Check PN bounds
    if ((PN < 1) || (PN > 8)) {
        printf("Invalid PN value (%d). PN needs to be between 1 and 8!\n", PN);
        exit(1);
    }

    // Generate randomized file
    int status = genRandomVals(L, H);
    if (!status) {
        printf("Random generation complete.\n");
    }
    else {
        printf("Error! Random file could not be generated!\n");
        return 1;
    }

    // Open generated random file and read vals into array
    fptr = fopen("generatedList.txt", "r");
    if (!fptr) { // Error on file open
        printf("Error! Random generated file could not be opened!\n");
        return 2;
    }
    int randVals[L];
    int bufferLen = 255;
    char buffer[bufferLen];
    int i = 0;
    while(fgets(buffer, bufferLen, fptr)) { // Reads values into randVals from file
        randVals[i] = atoi(buffer);
        i++;
    }   
    /*for (int i = 0; i < L; i++) {
        printf("Value #%d: %d\n", i+1, randVals[i]);
    }*/
    fclose(fptr);

    // Calculate split points for array
    int splitPoints[PN];
    int splitSize = L / PN;
    printf("Split size: %d\n", splitSize);
    splitPoints[0] = 0;
    splitPoints[PN] = L;
    for (int i = 1; i < PN; i++) {
        splitPoints[i] = i * splitSize;
    }
    for (int i = 0; i < PN+1; i++) {
        printf("Split point #%d: %d\n", i, splitPoints[i]);
    }
    // TODO: Split randVals into subprocess arrays

    // TODO: Perform BFS search
    int pipefd[PN][2];
    int status[PN]; 
    pid_t pid = 1;
    printf("Beginning BFS key search...\n");

    /*
    for (int i = 0; (i < PN) && pid; i++) {
        // Create pipe for ith child process
        if (pipe(pipefd[i]) == -1) {
            printf("Error! Pipe could not be created!\n");
            return 3;
        }

        // Create new processes
        pid_t pid = fork();
        if (pid == -1) {
            printf("Error! Fork could not be performed!\n");
            return 4;
        }
        else if (pid == 0) { // Child process
            
            close(pipefd[i][0]);
        }
        else { // Parent process
            close(pipefd[i][1]);
            waitpid(pid,&status[i],0);
        }
    */


    // TODO: Perform DFS search

    // Return
    return 0;
}