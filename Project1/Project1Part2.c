#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
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

// BFS Implementation
int BFSSearch(int PN, int *splitPoints, int *randVals){
    // Set up BFS time measurements
    clock_t startBFS, endBFS;
    double cpu_time_used_bfs;
    startBFS = clock();

    // Perform BFS search
    int pipefdwrite[PN][2]; // Child -> Parent
    int status[PN];        // Store status of child processes
    int keyCount[PN];       // Store key count for each child
    int max[PN];            // Store max key for each child
    int avg[PN];            // Store avg key value for each child
    int totalKeys = 0;
    int maxKey = 0;
    int avgKey = 0;
    pid_t pid = 1;
    printf("\nBeginning BFS key search...\n");
    for (int i = 0; (i < PN) && pid; i++) {
        // Create pipe for ith child process
        if (pipe(pipefdwrite[i]) == -1) {
            printf("Error! Pipe could not be created!\n");
            return 3;
        }

        // Create new processes
        pid_t pid = fork();
        if (pid == -1) { // Error on fork()
            printf("Error! Fork could not be performed!\n");
            return 4;
        }
        else if (pid == 0) { // Child process
            // Print out process info
            pid_t childPID = getpid();
            pid_t parentPID = getppid();
            printf("Child process #%d created with PID: %d and Parent PID: %d\n", i, childPID, parentPID);

            int keyCount = 0;
            int max = 0;
            int avg = 0;
            int startPoint = splitPoints[i];
            int endPoint = splitPoints[i+1];
            //int len = endPoint - startPoint;
            close(pipefdwrite[i][0]);

            for(int j = startPoint; j < endPoint; j++){
                // Checks iteratively if its a key value and counts it.
                if (randVals[j] < 0) {
                    ++keyCount;

                    // Change to max if the previous max is smaller (in magnitude) than current key.
                    if (max > (randVals[j])) {
                        max = randVals[j];
                    }

                    // Sum all the keys iteratively
                    avg += randVals[j];
                }
            }
            avg = avg / keyCount;

            // Write keyCount, max, and avg to parent
            write(pipefdwrite[i][1], &keyCount, sizeof(keyCount));
            write(pipefdwrite[i][1], &max, sizeof(max));
            write(pipefdwrite[i][1], &avg, sizeof(avg));
            close(pipefdwrite[i][1]);

            // Raise SIGSTP
            raise(SIGTSTP);
            

            // Exit child process
            exit(0);
            return 0; 
        }
        else { // Parent process
            // Close writing end for parent
            close(pipefdwrite[i][1]);

            // Read data from child
            read(pipefdwrite[i][0], &keyCount[i], sizeof(keyCount[i]));
            read(pipefdwrite[i][0], &max[i], sizeof(max[i]));
            read(pipefdwrite[i][0], &avg[i], sizeof(avg[i]));

            // Inspect BFS process tree
            //int pstreeCode = system("pstree -p");
            waitpid(pid, &status[i], 0);
        }
    }
    for (int i = 0; i < PN; i++) { // TODO: Update BFS average calculation
        totalKeys += keyCount[i];
        avgKey += avg[i];
        if (maxKey > max[i]) {
            maxKey = max[i];
        }
    }
    avgKey = -1 * (avgKey / PN);
    maxKey *= -1;
    printf("Total keys found: %d\n", totalKeys);
    printf("Max key found: %d\n", maxKey);
    printf("Avg key value found: %d\n", avgKey);

    // Measure time taken by parallelized BFS code
    endBFS = clock();
    cpu_time_used_bfs = ((double) (endBFS - startBFS)) / CLOCKS_PER_SEC;
    printf("BFS time duration: %lf\n", cpu_time_used_bfs);
    return 0;
}

// DFS Implementation
int DFSSearch(int PN, int *splitPoints, int *randVals, int (*pipefdwrite)[2], int depth){
    if(depth >= PN) { // Too many processes created
        return -1;
    }

    //if (depth < PN-1) {
    pid_t PID = fork();
    //}
    
    if (PID == -1) { // Error on fork()
        printf("Error! Fork could not be performed!\n");
        return 4;
    }
    else if (PID == 0) { // Child process
        // Increment depth
        depth++;

        // Print out process info
        pid_t childPID = getpid();
        pid_t parentPID = getppid();
        printf("Child process #%d created with PID: %d and Parent PID: %d\n", depth, childPID, parentPID);

        int localkey = 0;
        int localmax = 0;
       	int localavg = 0;
        int startPoint = splitPoints[depth-1];
        int endPoint = splitPoints[depth];
        //int len = endPoint - startPoint;

        for(int j = startPoint; j < endPoint; j++) {
            // Checks iteratively if its a key value and counts it.
            if (randVals[j] < 0) {
                ++localkey;

                // Change to max if the previous max is smaller (in magnitude) than current key.
                if (localmax > (randVals[j])) {
                    localmax = randVals[j];
                }

                // Sum all the keys iteratively
                localavg += randVals[j];
            }
        }
        localavg = localavg / localkey;

        // Write keyCount, max, and avg to parent
        write(pipefdwrite[depth-1][1], &localkey, sizeof(localkey));
        //printf("Write Keycount of PID %d: %d\n",childPID, localkey);
        write(pipefdwrite[depth-1][1], &localmax, sizeof(localmax));
        //printf("Write Max of PID %d: %d\n",childPID, localmax);
        write(pipefdwrite[depth-1][1], &localavg, sizeof(localavg));
        //printf("Write Avg of PID %d: %d\n",childPID, localavg);
        close(pipefdwrite[depth-1][1]);

        //Recursively create leaves.
        if (depth < PN) {
            DFSSearch(PN, splitPoints, randVals, pipefdwrite, depth);
        }

        // Exit child process
        exit(0);
        return 0; 
    }
    else { // Parent process
        //system("pstree -p");
        int status; // Store status of child process
        //printf("Parent process %d waiting on process %d\n", getpid(), PID);
        waitpid(PID, &status, 0);
    }

    if (depth == 0 && PID != 0) { // If parent node, do calculations associated to entire process, else don't execute this.
        int keyCount[PN];       // Store key count for each child
        int max[PN];            // Store max key for each child
        int avg[PN];            // Store avg key value for each child
        int totalKeys = 0;
        int maxKey = 0;
        int avgKey = 0;
        for (int i = 0; i < PN; i++) {
            // Read data from child
            read(pipefdwrite[i][0], &keyCount[i], sizeof(keyCount[i]));
            //printf("Read Keycount of PID %d: %d\n", PID, keyCount[i]);
            read(pipefdwrite[i][0], &max[i], sizeof(max[i]));
            //printf("Read Max of PID %d: %d\n", PID, max[i]);
            read(pipefdwrite[i][0], &avg[i], sizeof(avg[i]));
            //printf("Read Avg of PID %d: %d\n", PID, avg[i]);
            // Inspect DFS process tree
            // int pstreeCode = system("pstree -p");
        }

        for (int i = 0; i < PN; i++) { // TODO: Update DFS average calculation
            //printf("avg #%d: %d\n", i, avg[i]);
            totalKeys += keyCount[i];
            avgKey += avg[i];
            if (max[i] < maxKey) {
                maxKey = max[i];
            }
        }

        avgKey = -1 * (avgKey / PN);
        maxKey *= -1;
        printf("Total keys found: %d\n", totalKeys);
        printf("Max key found: %d\n", maxKey);
        printf("Avg key value found: %d\n", avgKey);
    }
    return 0;
}

// DFS Initialization Function
int DFS(int PN, int *splitPoints, int *randVals) {
    int pipefdwrite[PN][2]; // Child -> Parent
    //int status[PN];        // Store status of child processes

    // Create pipe for the child process
    for (int i = 0; i < PN; i++) {
        if (pipe(pipefdwrite[i]) == -1) {
            printf("Error! Pipe could not be created!\n");
            return 3;
        }
    }

    // Begin DFS search
    DFSSearch(PN, splitPoints, randVals, pipefdwrite, 0);

    return 0;
}

// MAIN
int main(int argc, char *argv[]) {
    FILE *fptr;

    // Set up serial time measurements
    clock_t startSerial, endSerial;
    double cpu_time_used_serial;
    startSerial = clock();

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
    int genReturnCode = genRandomVals(L, H);
    if (!genReturnCode) {
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

    // Statically calculate split points for array
    int splitPoints[PN+1];
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

    // Measure time taken by serial (non-parallel) code
    endSerial = clock();
    cpu_time_used_serial = ((double) (endSerial - startSerial)) / CLOCKS_PER_SEC;
    printf("Serial time duration: %lf\n", cpu_time_used_serial);    // Return

    // Perform BFS search
    int BFSReturnCode = BFSSearch(PN, splitPoints, randVals);

    // Set up DFS time measurement
    clock_t startDFS, endDFS;
    double cpu_time_used_dfs;
    startDFS = clock();

    // Perform DFS search
    printf("\nBeginning DFS key search...\n");
    int DFSReturnCode = DFS(PN, splitPoints, randVals);

    // Measure time taken by parallelized BFS code
    endDFS = clock();
    cpu_time_used_dfs = ((double)(endDFS - startDFS)) / CLOCKS_PER_SEC;
    printf("DFS time duration: %lf\n", cpu_time_used_dfs);

    // Exit
 	return 0;
}
