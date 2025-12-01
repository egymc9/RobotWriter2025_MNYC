#include <stdio.h>
#include <stdlib.h>

#define SSF_SECTION_VALUE 999

int char_to_ascii(char ch)
{
    return (unsigned char)ch;
}

int main(void)
{
    FILE *fInput;

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

    //ASCII chars to be tested
    char test_chars[] = {'A', 'z', '0', '9', '!', '@', '#', '\n', '\t', ' '};
    int num_test_chars = sizeof(test_chars) / sizeof(test_chars[0]);
    int charseq[num_test_chars]; // Array to hold ASCII values

    // Convert characters to ASCII values
    for (i = 0; i < num_test_chars; i++)
    {
        char cha = test_chars[i];
        int ascii_value = char_to_ascii(cha);
        charseq[i] = ascii_value;
    }

    // The 2D array to hold the data from the Single Stroke Font data file
    int SSF[K][3];

    // Writing data from the file into the 2D array
    for ( i = 0 ; i < K ; i++)
    {
        fscanf (fInput, "%d %d %d", &SSF[i][0], &SSF[i][1], &SSF[i][2]);
    }

    // Close the file
    fclose (fInput);

    // Extract and print strokes for each test character
    for (i = 0; i < num_test_chars; i++)
    {
        for (j = 0; j < K; j++)
        {
            if (SSF[j][0] == SSF_SECTION_VALUE)
            {
                if (SSF[j][1] == charseq[i])
                {
                    int section_lines = SSF[j][2];
                    printf ("Char: %c, ASCII: %d, Strokes: %d\n",test_chars[i], SSF[j][1], SSF[j][2]);
                    for (k = 1; k <= section_lines; k++)
                    {
                        printf ("%d %d %d\n", SSF[j + k][0], SSF[j + k][1], SSF[j + k][2]);
                    }
                }
            }
        }
    }

    return (0);  
}