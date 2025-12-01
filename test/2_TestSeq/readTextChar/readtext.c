#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *fInput;

    // Variables
    int i =0 , k;

    // Open the text file for reading
    fInput = fopen ("test.txt", "r");

    // File validation
    if ( fInput == NULL)
    {
        printf ("\nthe file could not be opened for reading, exiting");
        return -1;
    }

    // Read file character by character into array
    int ch;
    
    // Count total number of characters in file
    int total_chars = 0;
    while ((ch = fgetc(fInput)) != EOF)
    {
        total_chars++;
    }
    rewind(fInput);  // Reset file pointer to beginning

    char buffer[total_chars + 1]; // +1 for null terminator
    i = 0;

    while ((ch = fgetc(fInput)) != EOF && i < sizeof(buffer) - 1)
    {
        buffer[i++] = (char)ch;
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
    fclose (fInput);
    return (0);  
}