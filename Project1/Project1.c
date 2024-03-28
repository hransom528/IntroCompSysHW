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

    // Attempt to open the file
    fptr = fopen("generatedlist.txt", "w");
    if (!fptr) {
        // TODO: Error on file open
        exit(1);
    }

    // TODO: Perform DFS or BFS

    // Close the file
    fclose(fptr);


    return 0;
}