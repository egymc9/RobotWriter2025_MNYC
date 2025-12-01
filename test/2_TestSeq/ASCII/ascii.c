#include <stdio.h>
#include <stdlib.h>

int char_to_ascii(char ch)
{
    return (unsigned char)ch;
}

int main()
{
    char test_chars[] = {'A', 'z', '0', '9', '!', '@', '#', '\n', '\t', ' '};
    int num_test_chars = sizeof(test_chars) / sizeof(test_chars[0]);
    int charseq[num_test_chars]; // Array to hold ASCII values

    for (int i = 0; i < num_test_chars; i++)
    {
        char cha = test_chars[i];
        int ascii_value = char_to_ascii(cha);
        charseq[i] = ascii_value;
        printf("Character: '%c' ASCII: %d\n", cha, ascii_value);
    }

    return 0;
}