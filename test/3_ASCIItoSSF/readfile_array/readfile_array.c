#include <stdio.h>
#include <stdlib.h>

#define SSF_SECTION_VALUE 999


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

    // The 2D array to hold the data from the Single Stroke Font data file
    // Column meaning:
    //  - For section header lines: SSF[i][0] == SSF_SECTION_VALUE
    //    SSF[i][1] == section index
    //    SSF[i][2] == number of lines in that section (or other metadata)
    //  - For coordinate lines: SSF[i][0] == x, SSF[i][1] == y, SSF[i][2] == pen up/down
    int SSF[K][3];
    int SSF_isSection[K]; // boolean-like array (0/1) to indicate rows that are section headers (first column == SSF_SECTION_VALUE)

    // Writing data from the file into the 2D array
    for ( i = 0 ; i < K ; i++)
    {
        fscanf (fInput, "%d %d %d", &SSF[i][0], &SSF[i][1], &SSF[i][2]);
        // Set section boolean based on first column being SSF_SECTION_VALUE (999)
        SSF_isSection[i] = (SSF[i][0] == SSF_SECTION_VALUE) ? 1 : 0;
        
        if (SSF_isSection[i])
        {
            printf ("(SECTION)  %d %d %d\n", SSF[i][0], SSF[i][1], SSF[i][2]);
        }
        else
        {
            for (j = 0; j < 3; j++) 
            {
                printf ("%d ",SSF[i][j]);
            }
            printf ("\n");
        }
    }

    // Close the file
    fclose (fInput);


    
    return (0);  
}