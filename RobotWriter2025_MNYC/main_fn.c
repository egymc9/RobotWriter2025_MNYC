#include <stdio.h>
#include <stdlib.h>
#include "main_fn.h"

// Function to read the single stroke data as 2D array from text file
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

    // Write data into the 2D array
    for (int i = 0; i < *rows; i++) 
    {
        SSF[i] = malloc(3 * sizeof(int));
        fscanf(fSSF, "%d %d %d", &SSF[i][0], &SSF[i][1], &SSF[i][2]);
    }
    fclose(fSSF);
    return SSF;
}

// Function to convert text from test.txt into char array
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

// Function to extract character array as SSF data
int extract_and_print_strokes(const char* buffer, int number_of_chars,int** SSF, int K, int stroke_data[][3],int init_height) 
{
    int stroke_index = 0, enter = 0, total_chars_out = 0;
    const int sep_value = 999; // Use 999 as separation value in this function
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

        // Measure upcoming word width
        int j = i;
        float word_width_mm = 0.0f;
        while (j < number_of_chars && buffer[j] != ' ' && buffer[j] != '\n') 
        {
            word_width_mm += init_height; // Approx width per char
            j++;
        }

        // If statement to check if word fits
        if (current_width_mm + word_width_mm > max_width_mm) 
        {
            // Move to next line BEFORE writing word
            enter++;
            total_chars_out = 0;
            current_width_mm = 0.0f;
        }

        // Output strokes for the word
        while (i < j) {
            for (int s = 0; s < K; s++) 
            {
                // If statement to check for the index row, find 999 and C
                if (SSF[s][0] == sep_value && SSF[s][1] == (unsigned char)buffer[i]) 
                {
                    // Read the N value in the index row
                    int section_lines = SSF[s][2];

                    // For loop to write stroke data array for N number of rows
                    for (int k = 1; k <= section_lines; k++) 
                    {
                        stroke_data[stroke_index][0] = SSF[s + k][0] + (18 * total_chars_out); // Add 18 to horizontal per character
                        stroke_data[stroke_index][1] = SSF[s + k][1] - (36 * enter); // Shift down writing by 36 per enter
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
