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
    FILE *fSSF;
    FILE *fText;

    // Variables
    int i, j, k;

    // Open the text file for reading
    fSSF = fopen ("SingleStrokeFont.txt", "r");

    // File validation
    if ( fSSF == NULL)
    {
        printf ("\nthe file could not be opened for reading, exiting");
        return -1;
    }

    // Count line breaks in the file
    k = 0;
    int ch;
    while ((ch = fgetc(fSSF)) != EOF)
    {
        if (ch == '\n')
        {
            k++;
        }
    }
    const int K = k+1;  // Total number of lines in the file including the last line which doesnt have a line break

    // Reset file pointer to beginning
    rewind(fSSF);

    // The 2D array to hold the data from the Single Stroke Font data file
    int SSF[K][3];

    // Writing data from the file into the 2D array
    for ( i = 0 ; i < K ; i++)
    {
        fscanf (fSSF, "%d %d %d", &SSF[i][0], &SSF[i][1], &SSF[i][2]);
    }

    // Close the file
    fclose (fSSF);


    // Open test.txt to read characters order
    fText = fopen ("test.txt", "r");

    // File validation
    if ( fText == NULL)
    {
        printf ("\nthe file could not be opened for reading, exiting");
        return -1;
    }

    int cah; // Temporary variable for number of characters in test.txt

    // Count total number of characters in file
    int total_chars = 0;
    while ((cah = fgetc(fText)) != EOF)
    {
        total_chars++;
    }
    rewind(fText);  // Reset file pointer to beginning for finding buffer

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


    int init_height = 0;
    float sum_char_width = 0.0;
    int max_width = 100;

    printf("enter initial height: \n"); 
    scanf("%d", &init_height);

    // after reading buffer and init_height
    int max_word_len = 0;
    // compute max_word_len by scanning buffer and counting characters between spaces/newlines
    float effective_height = init_height;
    if (max_word_len * effective_height > max_width)
        effective_height = max_width / (float)max_word_len;
    int chars_per_line = (int)(max_width / effective_height);



// Extract and print strokes for each test character
    // Create 2D array to store stroke data
    int stroke_data[K][3];
    int stroke_index = 0;
    
    int enter = 0;            // line index
    int total_chars_out = 0;  // horizontal character count within the current line

    for (i = 0; i < number_of_chars; i++) 
    {// If the input text has a newline, move to next line baseline
        if (buffer[i] == '\n') {
            enter++;
            total_chars_out = 0; // reset X position at start of new line
            continue;            // no glyph strokes for '\n'
        }

        // Find glyph in SSF array
        for (j = 0; j < K; j++) {
            if (SSF[j][0] == SSF_SECTION_VALUE && SSF[j][1] == (unsigned char)buffer[i]) 
            {
                int section_lines = SSF[j][2];
                for (k = 1; k <= section_lines; k++) {
                    stroke_data[stroke_index][0] = SSF[j + k][0] + (18 * total_chars_out);
                    stroke_data[stroke_index][1] = SSF[j + k][1] - (36 * enter); // line shift down
                    stroke_data[stroke_index][2] = SSF[j + k][2];
                    stroke_index++;
                }
                total_chars_out++; // advance horizontal position after placing the character
                break;
            }
        }
    }

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
        if (stroke_data[i][2] == 1)
            fprintf(fGCode, "S1000\nG1 ");
        else
            fprintf(fGCode, "S0\nG0 ");

        float x_pos = int_to_float(stroke_data[i][0]) * init_height / 18.0f;
        float y_pos = int_to_float(stroke_data[i][1]) * init_height / 18.0f;

        fprintf(fGCode, "X%f Y%f\n", x_pos, y_pos);
    }


    sum_char_width = (total_chars_out * init_height / 18)*18;

    fprintf(fGCode, "G0 Z0\n");  // Lift tool
    fprintf(fGCode, "M30\n");    // End program

    fclose(fGCode);
    printf("\nG-code written to output.gcode\n");


    return (0);  
}
