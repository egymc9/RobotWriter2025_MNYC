#include <stdio.h>
#include <stdlib.h>
#include "main_fn.h"

int main() 
{
    // Use the read_SSF function to convert SingleStrokeFont.txt into a pointer array
    int rows;
    int** data = read_SSF("SingleStrokeFont.txt", &rows);

    // Use function to buffer the text from test.txt into ASCII pointer array.
    int length;
    char* buffer_seq = file_to_buffer("test.txt", &length);

    if (data && buffer_seq) // If statement for checking if both pointers above are available
    {
        // Input initial height of the printing
        int init_height;
        printf("Enter initial height: ");
        scanf("%d", &init_height);

        int (*stroke_data)[3] = malloc(rows * sizeof *stroke_data);
        if (!stroke_data) 
        { 
            printf("Memory allocation failed\n"); 
            return -1; 
        }

        // Use function to extract the ASCII array as SSF stroke data
        int strokes = extract_and_print_strokes(buffer_seq, length, data, rows, stroke_data, init_height);


        // Temporary section to preview G Code as an actual file
        FILE *fGCode = fopen("output.gcode", "w");
        if (!fGCode) 
        { 
            printf("Error opening output.gcode\n"); 
            return -1; 
        }

        fprintf(fGCode, "F1000\nM3\nS0\n");

        for (int i = 0; i < strokes; i++) 
        {
            if (stroke_data[i][2] == 1) fprintf(fGCode, "S1000\nG1 ");
            else fprintf(fGCode, "S0\nG0 ");

            float x_pos = (float)stroke_data[i][0] * init_height / 18.0f;
            float y_pos = (float)stroke_data[i][1] * init_height / 18.0f;
            fprintf(fGCode, "X%f Y%f\n", x_pos, y_pos);
        }

        fprintf(fGCode, "G0 Z0\nM30\n");
        fclose(fGCode);
        printf("G-code written to output.gcode\n");

        //Free allocated memory
        free(stroke_data);
        free(buffer_seq);
        for (int i = 0; i < rows; i++) free(data[i]);
        free(data);
    }
    return 0;
}