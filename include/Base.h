#pragma once

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct 
{
    size_t length;
    const char *m_src;
    const char *file_path;
} File;

int Exit_With_Error(const char *m, FILE *f, char * buffer);

char *CheckFilext(const char *File_Path) {
    char *dot = strrchr(File_Path, '.');
    if (!dot || dot == File_Path) {
        return "";
    }
    return dot;   
}

void Open_Read_File(File *f) 
{
    FILE *file = fopen(f->file_path, "rb");
    if (!file) 
    {
        Exit_With_Error("Could not open file", NULL, NULL);
        return;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    f->length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate buffer and read content
    char *buffer = malloc(f->length + 1);
    fread(buffer, 1, f->length, file);
    buffer[f->length] = '\0';
    
    // Assign to file struct
    f->m_src = buffer;
    fclose(file);
}
