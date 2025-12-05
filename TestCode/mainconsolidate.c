#include <stdio.h>
#include <stdlib.h>

// Function to read a file into a 2D array of 3 columns
int** read_SSF(const char* filename, int* rows) 
{
    FILE* fSSF = fopen(filename, "r");
    if (fSSF == NULL) 
    {
        printf("Error: could not open file %s\n", filename);
        *rows = 0;
        return NULL;
    }

    // Count line breaks to determine number of rows
    int k = 0;
    int ch;
    while ((ch = fgetc(fSSF)) != EOF) 
    {
        if (ch == '\n') 
        {
            k++;
        }
    }
    *rows = k + 1;  // include last line without newline

    // Reset file pointer
    rewind(fSSF);

    // Allocate 2D array dynamically
    int** SSF = (int**)malloc((*rows) * sizeof(int*));
    if (SSF == NULL) 
    {
        printf("Memory allocation failed\n");
        fclose(fSSF);
        *rows = 0;
        return NULL;
    }
    for (int i = 0; i < *rows; i++) 
    {
        SSF[i] = (int*)malloc(3 * sizeof(int));
        if (SSF[i] == NULL) 
        {
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
    for (int i = 0; i < *rows; i++) 
    {
        fscanf(fSSF, "%d %d %d", &SSF[i][0], &SSF[i][1], &SSF[i][2]);
    }

    fclose(fSSF);
    return SSF;
}

// Function to read Test.txt and convert into a char array
int* file_to_ascii(const char* filename, int* length) 
{
    FILE* fText = fopen(filename, "r");
    if (fText == NULL) {
        printf("Error: could not open file %s\n", filename);
        *length = 0;
        return NULL;
    }

    // Count total characters
    int total_chars = 0;
    int cah;
    while ((cah = fgetc(fText)) != EOF) 
    {
        total_chars++;
    }
    rewind(fText);

    // Allocate buffer and ASCII array
    char* buffer = (char*)malloc((total_chars + 1) * sizeof(char));
    int* charseq = (int*)malloc(total_chars * sizeof(int));
    if (buffer == NULL || charseq == NULL) 
    {
        printf("Error: memory allocation failed\n");
        fclose(fText);
        *length = 0;
        return NULL;
    }

    // Read file into buffer
    int i = 0;
    while ((cah = fgetc(fText)) != EOF && i < total_chars) 
    {
        buffer[i++] = (char)cah;
    }
    buffer[i] = '\0';

    // Convert to ASCII values
    for (int k = 0; k < total_chars; k++) 
    {
        charseq[k] = (unsigned char)buffer[k];
    }

    // Cleanup
    fclose(fText);
    free(buffer);

    *length = total_chars;  // return length via pointer
    return charseq;         // return ASCII array
}




int main() {
    int rows;
    int** data = read_SSF("SingleStrokeFont.txt", &rows);

    int length;
    int* ascii_array = file_to_ascii("test.txt", &length);

    return 0;
}