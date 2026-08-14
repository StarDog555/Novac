#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

bool Debug = false;
bool Compiled = false;

#include "include/Base.h"
#include "include/lexer.h"
#include "include/Parser.h" 

char *Outname = "Default";

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
    if (argc < 2) {
        return Exit_With_Error("No Input File\n", NULL, NULL);
    }

    int FileCount = 0;
    File file = {0};

    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc) {
                return Exit_With_Error("No OutName After -o\n", NULL, NULL);
            }

            if (argv[i + 1][0] == '-') {
                return Exit_With_Error("Invalid OutName After -o\n", NULL, NULL);
            }

            Outname = argv[++i];
        }

        else if (strcmp(argv[i], "--compile") == 0) {
            if (i + 1 < argc) {
                return Exit_With_Error(
                    "Flags Like --compile Need To Be At The End\n",
                    NULL,
                    NULL
                );
            }

            Compiled = true;
        }

        else if (strcmp(argv[i], "--debug") == 0) {
            Debug = true;
        }

        else if (strcmp(argv[i], "--help") == 0) {
            printf("TODO: Give & Open Documentation Link (Local)\n");
            return EXIT_SUCCESS;
        }

        else if (strcmp(CheckFilext(argv[i]), ".nv") == 0) {
            if (FileCount >= 1) {
                return Exit_With_Error(
                    "Too many Input Files, Try Importing instead\n",
                    NULL,
                    NULL
                );
            }

            file.file_path = argv[i];
            Open_Read_File(&file);

            FileCount++;
        }

        else {
            return Exit_With_Error(
                "Unknown Command or Invalid File Type, Try --help\n",
                NULL,
                NULL
            );
        }
    }

    if (FileCount == 0) {
        return Exit_With_Error("No Input File\n", NULL, NULL);
    }

    if (Compiled) {
        m_index = 0;
        Line_count = 1;
        col_count = 1;

        Tokenize(&m_index, &file);
        Parse(tokens, Token_count);
    }

    if (Debug) {
        printf("DEBUG: Input File: %s\n", file.file_path);
        printf("DEBUG: File Length: %zu\n", file.length);
        
        if (strcmp(Outname, "Default") == 0) {
            printf("DEBUG: No Output File Name Enter Set To Default\n");
        } else {
            printf("DEBUG: Output File Name: %s\n", Outname);   
        }
        
        printf("DEBUG: Compiled: %s\n", Compiled ? "true" : "false");

        if (Compiled) {
            printf("DEBUG: Total Tokens: %d\n", Token_count);

            for (int j = 0; j < Token_count; j++) {
                printf(
                    "Token %d -> Type: %s, Value: %d, Row: %d, Col: %zu\n",
                    j,
                    token_type_to_string(tokens[j].type),
                    tokens[j].value,
                    tokens[j].row,
                    tokens[j].col
                );
            }
            
            printf("\n--- EXPE TREE ---\n");
            Print_EXPE(temp->E, 0);
        }
    }

    return EXIT_SUCCESS;
}
