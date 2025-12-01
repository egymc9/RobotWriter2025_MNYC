#include <stdio.h>
#include <stdlib.h>

int char_to_ascii(char ch)
{
    return (unsigned char)ch;
}

int main(void)
{
//ReadText section
    FILE *fInput;

    // Variables
    int i, k;

    // Open the text file for reading
    fInput = fopen ("test.txt", "r");

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

    char TextLines[K][100]; // The 2D array to hold the data from the text file

    // Writing data from the file into the 2D array
    for ( i = 0 ; i < K ; i++)
    {
        fgets (TextLines[i], sizeof(TextLines[i]), fInput);
        
        printf("%s",TextLines[i]);
    }

    printf("\n\n");

    // Close the file
    fclose (fInput);

// ASCII section
    int num_test_chars = sizeof(TextLines) / sizeof(TextLines[0]);
    for (int j = 0; j < num_test_chars; j++)
    {
        for (int m = 0; TextLines[j][m] != '\0'; m++)
        {
            char cha = TextLines[j][m];
            int ascii_value = char_to_ascii(cha);
            printf("Character: '%c' ASCII: %d\n", cha, ascii_value);

        }
    }


    return (0);  
}