#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "http_server.h"


///////////////////////////////////////////////
//
//          FILES
//
///////////////////////////////////////////////

uint8_t open_file(const char *filename, http_file_t *hfile)
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
        return (1);
    
    fseek(fp, 0L, SEEK_END);
    hfile->size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    
    hfile->buf = (char *)malloc((sizeof(char) * hfile->size) + 1);

    if (fread(hfile->buf, sizeof(char), hfile->size, fp) != hfile->size)
        return (1);
    
    hfile->buf[hfile->size] = 0;
    
    fclose(fp);
    
    fp = NULL;

    return (0);
}

void close_file(http_file_t *hfile)
{
    if (hfile->buf)
        free(hfile->buf);
    
    hfile->buf = NULL;
}