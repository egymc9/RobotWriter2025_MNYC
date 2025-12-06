#include <stdio.h>
#include <stdlib.h>

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

    // Copy data into the 2D array
    for (int i = 0; i < *rows; i++) 
    {
        SSF[i] = malloc(3 * sizeof(int));
        fscanf(fSSF, "%d %d %d", &SSF[i][0], &SSF[i][1], &SSF[i][2]);
    }
    fclose(fSSF);
    return SSF;
}

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
    char* buffer = malloc((total_chars + 1) * sizeof(char));

    // Read ASCII values from file into buffer
    int i = 0;
    while ((c = fgetc(fText)) != EOF) 
    {
        buffer[i++] = (char)c;
    }
    buffer[i] = '\0';

    fclose(fText);
    *length = total_chars;
    return buffer;
}

int extract_and_print_strokes
(
    const char* buffer, 
    int number_of_chars,          
    int** SSF, 
    int K, 
    int stroke_data[][3],                    
    int init_height
) 
{
    int stroke_index = 0, enter = 0, total_chars_out = 0;
    const int sep_value = 999;
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
            current_width_mm += init_height; // space width
            i++; 
            continue;
        }

        //Measure upcoming word width
        int j = i;
        float word_width_mm = 0.0f;
        while (j < number_of_chars && buffer[j] != ' ' && buffer[j] != '\n') 
        {
            word_width_mm += init_height; // approx width per char
            j++;
        }

        // Check if word fits
        if (current_width_mm + word_width_mm > max_width_mm) 
        {
            // move to next line BEFORE writing word
            enter++;
            total_chars_out = 0;
            current_width_mm = 0.0f;
        }

        //Output strokes for the word
        while (i < j) {
            for (int s = 0; s < K; s++) {
                if (SSF[s][0] == sep_value && SSF[s][1] == (unsigned char)buffer[i]) 
                {
                    int section_lines = SSF[s][2];
                    for (int k = 1; k <= section_lines; k++) 
                    {
                        stroke_data[stroke_index][0] = SSF[s + k][0] + (18 * total_chars_out);
                        stroke_data[stroke_index][1] = SSF[s + k][1] - (36 * enter);
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

int main() 
{
    // Use the read_SSF function to convert SingleStrokeFont.txt into a pointer array
    int rows;
    int** data = read_SSF("SingleStrokeFont.txt", &rows);

    // Use function to buffer the text from test.txt into ASCII pointer array.
    int length;
    char* buffer = file_to_buffer("test.txt", &length);

    if (data && buffer) // If statement for checking if both pointers above are available
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
        int strokes = extract_and_print_strokes(buffer, length, data, rows, stroke_data, init_height);


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
        free(buffer);
        for (int i = 0; i < rows; i++) free(data[i]);
        free(data);
    }
    return 0;
}