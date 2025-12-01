#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (void)
{
    int numbers[] = {999, 1, 2, 3, 4, 5, 6, 7, 999, 8, 9, 10, 11, 12, 13, 14, 15, 999, 16, 17, 18, 19, 20};
    int n = sizeof(numbers) / sizeof(numbers[0]);

    #define SSF_SECTION_VALUE 999
    /* Split the array at every occurrence of SSF_SECTION_VALUE */
    int sep = SSF_SECTION_VALUE; /* separator value */

    /* First pass: count segments (skip empty segments created by consecutive separators) */
    int seg_count = 0;
    for (int i = 0; i < n;)
    {
        /* skip separators */
        while (i < n && numbers[i] == sep) i++;
        if (i >= n) break;
        /* found start of a segment */
        seg_count++;
        /* advance to next separator */
        while (i < n && numbers[i] != sep) i++;
    }

    if (seg_count == 0)
    {
        printf("No segments found.\n");
        return 0;
    }

    /* Allocate arrays to hold pointers to segments and their lengths */
    int **segments = malloc(seg_count * sizeof(int *));
    int *lengths = malloc(seg_count * sizeof(int));
    if (!segments || !lengths)
    {
        fprintf(stderr, "Allocation failed\n");
        free(segments); free(lengths);
        return 1;
    }

    /* Second pass: extract segments */
    int seg_index = 0;
    for (int i = 0; i < n;)
    {
        while (i < n && numbers[i] == sep) i++;
            if (i >= n) break;
        
        int start = i;

        while (i < n && numbers[i] != sep) i++;
        int len = i - start;
        lengths[seg_index] = len;
        segments[seg_index] = malloc(len * sizeof(int));
        if (!segments[seg_index])
        {
            fprintf(stderr, "Allocation failed for segment %d\n", seg_index);
            /* cleanup */
            for (int j = 0; j < seg_index; ++j) free(segments[j]);
            free(segments); free(lengths);
            return 1;
        }
        for (int k = 0; k < len; ++k) segments[seg_index][k] = numbers[start + k];
        seg_index++;
    }

    /* Print segments */
    for (int s = 0; s < seg_count; ++s)
    {
        printf("Segment %d (length %d):\n", s, lengths[s]);
        for (int i = 0; i < lengths[s]; ++i)
        {
            printf("%d ", segments[s][i]);
        }
        printf("\n");
    }

    /* Free memory */
    for (int s = 0; s < seg_count; ++s) free(segments[s]);
    free(segments);
    free(lengths);

    return 0;
}
