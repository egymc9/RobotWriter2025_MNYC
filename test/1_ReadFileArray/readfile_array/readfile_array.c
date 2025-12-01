#include <stdio.h>
#include <stdlib.h>


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

    int SSF[K][3]; // The 2D array to hold the data from the Single Stroke Font data file

    // Writing data from the file into the 2D array
    for ( i = 0 ; i < K ; i++)
    {
        fscanf (fInput, "%d %d %d", &SSF[i][0], &SSF[i][1], &SSF[i][2]);
        
        for (j = 0; j < 3; j++) 
        {
            printf ("%d ",SSF[i][j]);
        }
        printf ("\n");
    }

    // Close the file
    fclose (fInput);
    return (0);  
}