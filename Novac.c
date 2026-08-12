#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "include/Base.h"
#include "include/lexer.h"

char *Outname;

int Exit_With_Error(const char *m, FILE *f, char *buffer)
{
    if (f != NULL)
        fclose(f);

    if (buffer != NULL)
        free(buffer);

    fprintf(stderr, "[-] ERROR: %s\n", m);
    return EXIT_FAILURE;
}

int main(int argc, char *argv[]) 
{
    fflush(stdout);
     
    // Giving Error If no Input Files
    if (argc < 2) {
        Exit_With_Error("No Input File\n", NULL, NULL);
        return 1;
    }

    // Needed For Checking
    int FileCount = 0;
    File file = {0};
    
    // Looping for argvs Checking Argv[i]
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc) {
                Exit_With_Error("No OutName After -o\n", NULL, NULL);
            }
            
            Outname = argv[++i];
        }
        else if (strcmp(argv[i], "-compile") == 0) {
            if (i + 1 < argc) {
                Exit_With_Error("Flags Like -compile Need To Be At The End", NULL                ,NULL);
            }
            Tokenize(&m_index, &file);

            int token_count = Token_count;
            
            // Loop through and print all tokens
            for (int j = 0; j < token_count; j++) 
            {
                printf("Token %d -> Type: %s, Value: %d, Line: %d\n",
                j,
                token_type_to_string(tokens[j].type),
                tokens[j].value,
                tokens[j].line);
            }
        }
        
        else if (strcmp(argv[i], "--help") == 0) {
            printf("TODO: Give & Open Documentation Link (Local)");
        }
        
        else if (strcmp(CheckFilext(argv[i]), ".nv") == 0) {
            if (FileCount >= 2) {
                Exit_With_Error("Too many Input Files, Try Importing instead                      \n", NULL, NULL);
            }
        
            file.file_path = argv[i];
            Open_Read_File(&file);
            
            FileCount++;
        }
        
        else {
            Exit_With_Error("Unknown Command or invaild File Type, --help", NULL,            NULL);
        }
    }

    return EXIT_SUCCESS;
}
