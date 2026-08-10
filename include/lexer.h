#pragma once

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "Base.h"

size_t m_index = 0;

typedef enum 
{
    TOKEN_INT_LIT, // Int Number
    TOKEN_EXIT, // exit
    TOKEN_SEMI, // ;
    TOKEN_EQ, // =
    TOKEN_EQEQ, // ==
    TOKEN_NEQ, // !=
    TOKEN_LN, // >
    TOKEN_MN, // <
    TOKEN_MNQ, // <=
    TOKEN_LNQ, // >=
    TOKEN_BRACP, // {
    TOKEN_BRACC, // }
    TOKEN_PRAO, // (
    TOKEN_PRAC, // )
    TOKEN_AND, // &&
    TOKEN_OR, // ||
    TOKEN_RETURN, // return
    TOKEN_IF, // if
    TOKEN_ELSE, // else
    TOKEN_ELSEIF, // else if
    TOKEN_FUNCTION, // fn 
    TOKEN_NULL, // NULL
    TOKEN_IMPORT, // inport
    TOKEN_INPUT, // input
    TOKEN_INVALID  // Invaild
} TokenType;

typedef struct
{
    TokenType type;
    int value;
    int line;
} Token;

int Token_count = 0;
int Line_count = 1;

char peek(int offset){ return m_src[m_index + offset];}
void consume(size_t *m_index) { (*m_index)++; }

void Tokenize(size_t *m_index, char *m_src) 
{
    if (m_src == NULL) {return;}
    if (Tokenlength <= 0) {return;}

    Token tokens[Tokenlength];
    
    while (*m_index < Tokenlength) 
    {
        char *buffer = malloc(Tokenlength + 1);
        int buffer_index = 0;

        if (buffer != NULL) 
        {
            while (isalpha(m_src[*m_index])) {
                buffer[buffer_index++] = m_src[*m_index];
                (*m_index)++;
            }
            
            buffer[buffer_index] = '\0';
            
            if (strcmp(buffer, "exit") == 0) {
                
                tokens[Token_count].type = TOKEN_EXIT;
                tokens[Token_count].value = 0;
                tokens[Token_count].line = Line_count;
                
                Token_count++;
            }

            else if (isspace(m_src[*m_index])) 
            {
                consume(m_index);
            }

            else if (isdigit(m_src[*m_index])) 
            {
                int value = 0;

                while (isdigit(m_src[*m_index])) {
                    value = value * 10 + (m_src[*m_index] - '0');
                    (*m_index)++;
                }

                
                tokens[Token_count].type = TOKEN_INT_LIT;
                tokens[Token_count].value = value;
                
                Token_count++;
            }

            else if (m_src[*m_index] == ';') 
            {
                
                tokens[Token_count].type = TOKEN_SEMI;
                tokens[Token_count].value = 0;
                tokens[Token_count].line = Line_count++;
                
                Token_count++;
                consume(m_index);
            }
            else 
            {
                tokens[Token_count].type = TOKEN_INVALID;
                tokens[Token_count].line = Line_count++;
                
                Token_count++;
                consume(m_index);
            }
            free(buffer);
        } else 
        {
            Exit_With_Error("Buffer is NULL\n", NULL, NULL);
            break;
        }
    }
}