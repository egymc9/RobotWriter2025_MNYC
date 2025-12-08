#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include "rs232.h"
#include "serial.h"

#include "main_fn.h"




#define bdrate 115200               /* 115200 baud */

void SendCommands (char *buffer );

int main()
{

    //char mode[]= {'8','N','1',0};
    char buffer[100];

    // If we cannot open the port then give up immediately
    if ( CanRS232PortBeOpened() == -1 )
    {
        printf ("\nUnable to open the COM port (specified in serial.h) ");
        exit (0);
    }

    //-

    // Use the read_SSF function to convert SingleStrokeFont.txt into a pointer array
    int rows;
    int** data = read_SSF("SingleStrokeFont.txt", &rows);

    // Use function to buffer the text from test.txt into ASCII pointer array.
    int length;
    char* buffer_seq = file_to_buffer("test.txt", &length);

    // Here is the main code:

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

        // Time to wake up the robot
        printf ("\nAbout to wake up the robot\n");

        // We do this by sending a new-line
        sprintf (buffer, "\n");
         // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
        PrintBuffer (&buffer[0]);
        Sleep(100);

        // This is a special case - we wait  until we see a dollar ($)
        WaitForDollar();

        printf ("\nThe robot is now ready to draw\n");

        //These commands get the robot into 'ready to draw mode' and need to be sent before any writing commands
        sprintf (buffer, "G1 X0 Y0 F1000\n");
        SendCommands(buffer);
        sprintf (buffer, "M3\n");
        SendCommands(buffer);
        sprintf (buffer, "S0\n");
        SendCommands(buffer);

        for (int i = 0; i < strokes; i++) 
        {
            int drawG;

            if (stroke_data[i][2] == 1) 
            {
                sprintf(buffer, "S1000\n");
                SendCommands(buffer);
                drawG = 1;
            }
                
            else 
            {
                sprintf(buffer, "S0\n");
                SendCommands(buffer);
                drawG = 0;
            }
            
            float x_pos = (float)stroke_data[i][0] * init_height / 18.0f;
            float y_pos = (float)stroke_data[i][1] * init_height / 18.0f;
            sprintf(buffer, "G%d X%f Y%f\n",drawG, x_pos, y_pos);
            SendCommands(buffer);
        }

        sprintf(buffer, "G0 Z0\nM30\n");
        SendCommands(buffer);

        //Free allocated memory
        free(stroke_data);
        free(buffer_seq);
        for (int i = 0; i < rows; i++) free(data[i]);
        free(data);
    }


    // Before we exit the program we need to close the COM port
    CloseRS232Port();
    printf("Com port now closed\n");

    return (0);
}

// Send the data to the robot - note in 'PC' mode you need to hit space twice
// as the dummy 'WaitForReply' has a getch() within the function.
void SendCommands (char *buffer )
{
    // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    WaitForReply();
    Sleep(100); // Can omit this when using the writing robot but has minimal effect
    // getch(); // Omit this once basic testing with emulator has taken place
}
