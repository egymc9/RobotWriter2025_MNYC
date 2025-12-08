#include <stdio.h>
#include <stdlib.h>
#include "main_fn.h"

int** read_SSF(const char* filename, int* rows) 
{
    FILE* fSSF = fopen(filename, "r");
    if (!fSSF) // Terminate if file is not available or restricted
    { 
        printf("Error opening %s\n", filename); 
        *rows = 0; 
        return NULL; 
    }

    // Count line breaks to determine number of rows
    int k = 0, ch;
    while ((ch = fgetc(fSSF)) != EOF) 
    {
        if (ch == '\n') 
        {
            k++;
        }
    }
    *rows = k + 1; // Include last line which doesn't have a newline

    // Reset file pointer
    rewind(fSSF);

    // Dynamic allocation for the 2D array
    int** SSF = malloc((*rows) * sizeof(int*));

    // Copy data into the 2D array
    for (int i = 0; i < *rows; i++) 
    {
        SSF[i] = malloc(3 * sizeof(int));
        fscanf(fSSF, "%d %d %d", &SSF[i][0], &SSF[i][1], &SSF[i][2]);
    }
    fclose(fSSF);
    return SSF;
}

char* file_to_buffer(const char* filename, int* length) 
{
    FILE* fText = fopen(filename, "r");
    if (!fText) // Terminate if file is not available or restricted
    { 
        printf("Error opening %s\n", filename); 
        *length = 0; 
        return NULL; 
    }

    // Count total characters
    int total_chars = 0, c;
    while ((c = fgetc(fText)) != EOF) 
    {
        total_chars++;
    }
    rewind(fText);

    // Allocate buffer and ASCII array
    char* buffer_f = malloc((total_chars + 1) * sizeof(char));

    // Read ASCII values from file into buffer
    int i = 0;
    while ((c = fgetc(fText)) != EOF) 
    {
        buffer_f[i++] = (char)c;
    }
    buffer_f[i] = '\0';

    fclose(fText);
    *length = total_chars;
    return buffer_f;
}

int extract_and_print_strokes(const char* buffer, int number_of_chars,int** SSF, int K, int stroke_data[][3],int init_height) 
{
    int stroke_index = 0, enter = 0, total_chars_out = 0;
    const int sep_value = 999;
    float current_width_mm = 0.0f;
    const float max_width_mm = 100.0f;

    for (int i = 0; i < number_of_chars; ) 
    {
        // Skip spaces/newlines directly
        if (buffer[i] == '\n') 
        {
            enter++; 
            total_chars_out = 0; 
            current_width_mm = 0.0f;
            i++; 
            continue;
        }
        if (buffer[i] == ' ') 
        {
            total_chars_out++; 
            current_width_mm += init_height; // Space width
            i++; 
            continue;
        }

        //Measure upcoming word width
        int j = i;
        float word_width_mm = 0.0f;
        while (j < number_of_chars && buffer[j] != ' ' && buffer[j] != '\n') 
        {
            word_width_mm += init_height; // approx width per char
            j++;
        }

        // Check if word fits
        if (current_width_mm + word_width_mm > max_width_mm) 
        {
            // move to next line BEFORE writing word
            enter++;
            total_chars_out = 0;
            current_width_mm = 0.0f;
        }

        //Output strokes for the word
        while (i < j) {
            for (int s = 0; s < K; s++) {
                if (SSF[s][0] == sep_value && SSF[s][1] == (unsigned char)buffer[i]) 
                {
                    int section_lines = SSF[s][2];
                    for (int k = 1; k <= section_lines; k++) 
                    {
                        stroke_data[stroke_index][0] = SSF[s + k][0] + (18 * total_chars_out);
                        stroke_data[stroke_index][1] = SSF[s + k][1] - (36 * enter);
                        stroke_data[stroke_index][2] = SSF[s + k][2];
                        stroke_index++;
                    }
                    total_chars_out++;
                    current_width_mm += init_height;
                    break;
                }
            }
            i++;
        }
    }
    return stroke_index;
}
