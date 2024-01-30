#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Validate program arguments.
    // *** YOUR CODE HERE. ***
    if ((argc < 3) || (argc > 4)) {
        printf("Usage: ./merge_files.c file1 file2 outFile");
        exit(0);
    }

    // Open two files to be merged.
    FILE *fp1 = fopen(argv[1], "r");
    FILE *fp2 = fopen(argv[2], "r");
    // Define third file, to be written to.
    FILE *fp3;

    // Open third file to store the result.
    // If only given 2 input filenames, third filename is "myfile.out" by
    // default.
    // If given 3 input filenames, third filename is the final argv.
    // *** YOUR CODE HERE. ***
    if (argc == 3) {
        fp3 = fopen("myfile.out", "w+");
    }
    else if (argc == 4) {
        fp3 = fopen(argv[3], "w+");
    }

    // Ensure all files exist.
    if (fp1 == NULL || fp2 == NULL || fp3 == NULL) {
        printf("No such file or directory.\n");
        exit(0);
    }

    // Copy contents of first file to file3.txt.
    // *** YOUR CODE HERE. ***
    char temp;
    do {
        temp = fgetc(fp1);
        fputc(temp, fp3);
    } while (temp != EOF);

    // Copy contents of second file to file3.txt.
    // *** YOUR CODE HERE. ***
    do {
        temp = fgetc(fp2);
        fputc(temp, fp3);
    } while (temp != EOF);

    // Close all files.
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    return 0;
}