#pragma once

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

size_t Tokenlength;
char *m_src;
char *file_contents;

typedef struct 
{
    char * buffer;
    size_t length;
    const char *file_path;
    const char *ext;
} File;

int Exit_With_Error(const char *m, FILE *f, char * buffer);

void CheckFilext(File *f) 
{
    const char *dot = strrchr(f->file_path, '.');
    if (!dot || dot == f->file_path) {
        return;
    }
    
    f->ext = dot;
    
    if (strcmp(f->ext, "nv") == 0) {} 
    else {
        Exit_With_Error("Wrong File Type: Enter A .nv File\n", NULL, NULL);
        return;
    }
}
void Open_Read_File(File *f) {
    f->buffer = 0;
    size_t length = f->length;
    FILE *file = fopen(f->file_path, "rb");

    if (file) {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        f->buffer = malloc(length + 1);

        if (f->buffer) {
            fread(f->buffer, 1, length, file);
            f->buffer[length] = '\0';
        }
        fclose(file);
    } else {
        Exit_With_Error("ERROR: File Not Found\n", file, f->buffer);
    }

    if (f->buffer) {
        file_contents = f->buffer, m_src = f->buffer, Tokenlength = length;
    }
}