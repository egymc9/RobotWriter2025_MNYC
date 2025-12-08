#ifndef MAIN_FUNCTIONS
#define MAIN_FUNCTIONS

#include <stdio.h>
#include <string.h>

int** read_SSF(const char*, int*);
char* file_to_buffer(const char*, int*);
int extract_and_print_strokes(const char*, int, int**, int, int (*)[3], int);

#endif