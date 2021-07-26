#include "http_server.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *filename, uint64_t *buffer_size)
{
    FILE *fp = fopen(filename, "rb");

    if (!fp)
        return (NULL);
    
    fseek(fp, 0L, SEEK_END);
    *buffer_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    char *heap = (char *)malloc((sizeof(char) * (*buffer_size) + 1));

    if (fread(heap, sizeof(char), *buffer_size, fp) != *buffer_size)
        return (NULL);
    
    fclose(fp);

    return (heap);
}