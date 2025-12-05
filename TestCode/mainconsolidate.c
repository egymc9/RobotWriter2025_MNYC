#include <stdio.h>
#include <stdlib.h>

// Function to read a file into a 2D array of 3 columns
int** read_SSF(const char* filename, int* rows) {
    FILE* fSSF = fopen(filename, "r");
    if (fSSF == NULL) {
        printf("Error: could not open file %s\n", filename);
        *rows = 0;
        return NULL;
    }

    // Count line breaks to determine number of rows
    int k = 0;
    int ch;
    while ((ch = fgetc(fSSF)) != EOF) {
        if (ch == '\n') {
            k++;
        }
    }
    *rows = k + 1;  // include last line without newline

    // Reset file pointer
    rewind(fSSF);

    // Allocate 2D array dynamically
    int** SSF = (int**)malloc((*rows) * sizeof(int*));
    if (SSF == NULL) {
        printf("Memory allocation failed\n");
        fclose(fSSF);
        *rows = 0;
        return NULL;
    }
    for (int i = 0; i < *rows; i++) {
        SSF[i] = (int*)malloc(3 * sizeof(int));
        if (SSF[i] == NULL) {
            printf("Memory allocation failed\n");
            // free already allocated rows
            for (int j = 0; j < i; j++) free(SSF[j]);
            free(SSF);
            fclose(fSSF);
            *rows = 0;
            return NULL;
        }
    }

    // Read data into the 2D array
    for (int i = 0; i < *rows; i++) {
        fscanf(fSSF, "%d %d %d", &SSF[i][0], &SSF[i][1], &SSF[i][2]);
    }

    fclose(fSSF);
    return SSF;
}

int main() {
    int rows;
    int** data = read_SSF("SingleStrokeFont.txt", &rows);

    if (data != NULL) {
        printf("File has %d rows:\n", rows);
        for (int i = 0; i < rows; i++) {
            printf("%d %d %d\n", data[i][0], data[i][1], data[i][2]);
        }

        // Free memory
        for (int i = 0; i < rows; i++) {
            free(data[i]);
        }
        free(data);
    }
    return 0;
}