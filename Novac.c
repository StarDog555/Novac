#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "include/Base.h"
#include "include/lexer.h"

int Exit_With_Error(const char *m, FILE *f, char * buffer) 
{
    if (f == NULL) {
        fprintf(stderr, "[-] ERROR: %s\n", m);
        return EXIT_FAILURE;
    }
    else if (buffer == NULL) {
        fprintf(stderr, "[-] ERROR: %s\n", m);
        return EXIT_FAILURE;
    }
    else if (f) { fclose(f); }
    else if (buffer) { free(buffer); }
    
    fprintf(stderr, "[-] ERROR: %s\n", m);
    return EXIT_FAILURE;
}

int main(int argc, char *argv[]) 
{
    char msg[2056];
    size_t msgsize = sizeof(msg);
    snprintf(msg, msgsize, "Usage: %s <FILEPATH>\n", argv[0]);
    
    if (argc < 2) {
        Exit_With_Error(msg, NULL, NULL);
        return 1;
    }
    File *f;

    f->file_path = argv[1];

    CheckFilext(f);
    Open_Read_File(f);
    Tokenize(&m_index, m_src);
    
    return EXIT_SUCCESS;
}