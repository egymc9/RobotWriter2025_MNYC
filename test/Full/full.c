#include <stdio.h>
#include <stdlib.h>

#define SSF_SECTION_VALUE 999

int char_to_ascii(char ch)
{
    return (unsigned char)ch;
}

float int_to_float(int value)
{
    return (float)value;
}

int main(void)
{
    FILE *fInput;
    FILE *fText;

    // Variables
    int i, j, k;

    // Open the text file for reading
    fInput = fopen ("SingleStrokeFont.txt", "r");

    // File validation
    if ( fInput == NULL)
    {
        printf ("\nthe file could not be opened for reading, exiting");
        return -1;
    }

    // Count line breaks in the file
    k = 0;
    int ch;
    while ((ch = fgetc(fInput)) != EOF)
    {
        if (ch == '\n')
        {
            k++;
        }
    }
    const int K = k+1;  // Total number of lines in the file including the last line which doesnt have a line break

    // Reset file pointer to beginning
    rewind(fInput);

    // The 2D array to hold the data from the Single Stroke Font data file
    int SSF[K][3];

    // Writing data from the file into the 2D array
    for ( i = 0 ; i < K ; i++)
    {
        fscanf (fInput, "%d %d %d", &SSF[i][0], &SSF[i][1], &SSF[i][2]);
    }

    // Close the file
    fclose (fInput);

    // Reopen text file to read characters

    // Open the text file for reading
    fText = fopen ("test.txt", "r");

    // File validation
    if ( fText == NULL)
    {
        printf ("\nthe file could not be opened for reading, exiting");
        return -1;
    }

    int cah;
    // Count total number of characters in file
    int total_chars = 0;
    while ((cah = fgetc(fText)) != EOF)
    {
        total_chars++;
    }
    rewind(fText);  // Reset file pointer to beginning

    char buffer[total_chars + 1]; // +1 for null terminator
    i = 0;

    while ((cah = fgetc(fText)) != EOF && i < sizeof(buffer) - 1)
    {
        buffer[i++] = (char)cah;
    }
    buffer[i] = '\0';  // Null terminate the string
    int number_of_chars = sizeof(buffer);
    int charseq[number_of_chars];

    // Convert characters to ASCII values
    for (k = 0; k < number_of_chars; k++)
    {
        char cha = buffer[k];
        int ascii_value = (unsigned char)cha;
        charseq[k] = ascii_value;
    }
    // Close the file
    fclose (fText);

// Extract and print strokes for each test character
    // Create 2D array to store stroke data
    int stroke_data[K][3];
    int total_chars_out = 0;
    int stroke_index = 0;
    
    for (i = 0; i < number_of_chars; i++)
    {
        for (j = 0; j < K; j++)
        {
            if (SSF[j][0] == SSF_SECTION_VALUE)
            {
                if (SSF[j][1] == charseq[i])
                {
                    int section_lines = SSF[j][2];
                    printf ("Char: %c, ASCII: %d, Strokes: %d\n", buffer[i], SSF[j][1], SSF[j][2]);
                    for (k = 1; k <= section_lines; k++)
                    {
                        stroke_data[stroke_index][0] = SSF[j + k][0] + (18 * total_chars_out); // Adjust x-coordinate
                        stroke_data[stroke_index][1] = SSF[j + k][1];
                        stroke_data[stroke_index][2] = SSF[j + k][2];
                        printf ("%d %d %d\n", stroke_data[stroke_index][0], stroke_data[stroke_index][1], stroke_data[stroke_index][2]);
                        stroke_index++;
                    }
                    total_chars_out++;
                }
            }
        }
    }

    int init_height = 0;
    float sum_char_width = 0.0;
    int max_width = 100;

    printf("enter initial height: \n"); 
    scanf("%d", &init_height);

    // Convert stroke data and export to G-code
    FILE *fGCode = fopen("output.gcode", "w");
    if (fGCode == NULL)
    {
        printf("\nthe file could not be opened for writing, exiting");
        return -1;
    }

    fprintf(fGCode, "F1000\n"); // Set feed rate
    fprintf(fGCode, "M3\n");   // Start spindle
    fprintf(fGCode, "S0\n");  // Set spindle speed to 0, pen up
    

    for (i = 0; i < stroke_index; i++)
    {
        // Check for word breaks (gap in x-coordinates)
        if (i > 0 && stroke_data[i][0] - stroke_data[i-1][0] > max_width)
        {
            // Move to the baseline of the next word instead of the absolute origin
            float next_x = int_to_float(stroke_data[i][0]) * init_height / 18;
            float next_y = 0.0; // baseline (Y=0)
            fprintf(fGCode, "S0\nG0 X%f Y%f\n", next_x, next_y);
        }

        if (stroke_data[i][2] == 1)
            fprintf(fGCode, "S1000\nG1 ");  // Pen down
        else
            fprintf(fGCode, "S0\nG0 ");     // Pen up
            
        float stroke_x = int_to_float(stroke_data[i][0]);
        float stroke_y = int_to_float(stroke_data[i][1]);

        float x_pos = stroke_x * init_height / 18;
        float y_pos = stroke_y * init_height / 18;

        fprintf(fGCode, "X%f Y%f\n", x_pos, y_pos);  // Move to position

        // Check if next character would exceed max width
        if (i + 1 < stroke_index && stroke_data[i + 1][0] - stroke_data[i][0] > max_width)
        {
            // Lookahead to ensure complete word fits
            int next_word_end = i + 1;
            while (next_word_end < stroke_index && stroke_data[next_word_end][0] - stroke_data[i + 1][0] <= max_width)
            {
                next_word_end++;
            }
            
            if (stroke_data[next_word_end][0] > max_width)
            {
                x_pos = 0;
                y_pos = y_pos + (init_height + 18); // Move down for next line
                fprintf(fGCode, "S0\nG0 X0 Y0\n");  // Pen up and move to next line
            }
        }
    }

    sum_char_width = (total_chars_out * init_height / 18)*18;

    fprintf(fGCode, "G0 Z0\n");  // Lift tool
    fprintf(fGCode, "M30\n");    // End program

    fclose(fGCode);
    printf("\nG-code written to output.gcode\n");


    return (0);  
}
