#pragma once

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "Base.h"

typedef enum 
{
    TOKEN_INT_LIT, // Int Number
    TOKEN_EXIT,    // exit
    TOKEN_SEMI,    // ;
    TOKEN_INVALID  // Invalid
} TokenType;

typedef struct
{
    TokenType type;
    int value;
    int line;
} Token;

size_t m_index = 0;
int Token_count = 0;
int Line_count = 1;
Token *tokens = NULL;

void consume(size_t *m_index_ptr) { (*m_index_ptr)++; }

char peek(int offset, File *f) { 
    return f->m_src[m_index + offset]; 
}

const char* token_type_to_string(TokenType type) 
{
    switch (type) 
    {
        case TOKEN_INT_LIT: return "TOKEN_INT_LIT";
        case TOKEN_EXIT:    return "TOKEN_EXIT";
        case TOKEN_SEMI:    return "TOKEN_SEMI";
        case TOKEN_INVALID: return "TOKEN_INVALID";
        default:            return "UNKNOWN";
    }
}

void Tokenize(size_t *m_index_ptr, File *f) 
{
    // --- DEBUG CHECK 1 ---
    if (f == NULL) {
        printf("DEBUG ERROR: File pointer 'f' is NULL!\n");
        return;
    }
    if (f->m_src == NULL) {
        printf("DEBUG ERROR: f->m_src is NULL! Open_Read_File failed or couldn't find 'test.nv'.\n");
        return; 
    }
    if (f->length <= 0) {
        printf("DEBUG ERROR: File length is %zu (empty file or failed to read).\n", f->length);
        return; 
    }

    tokens = malloc(sizeof(Token) * (f->length + 1));
    Token_count = 0;

    while (*m_index_ptr < f->length) 
    {
        char current = f->m_src[*m_index_ptr];

        if (isspace(current)) 
        {
            if (current == '\n') {
                Line_count++;
            }
            consume(m_index_ptr);
            continue;
        }

        if (isalpha(current)) 
        {
            char buffer[256];
            int buffer_index = 0;

            while (*m_index_ptr < f->length && isalpha(f->m_src[*m_index_ptr])) 
            {
                if (buffer_index < 255) {
                    buffer[buffer_index++] = f->m_src[*m_index_ptr];
                }
                (*m_index_ptr)++;
            }
            buffer[buffer_index] = '\0';

            if (strcmp(buffer, "exit") == 0) 
            {
                tokens[Token_count].type = TOKEN_EXIT;
                tokens[Token_count].value = 0;
                tokens[Token_count].line = Line_count;
            } 
            else 
            {
                tokens[Token_count].type = TOKEN_INVALID;
                tokens[Token_count].value = 0;
                tokens[Token_count].line = Line_count;
            }
            Token_count++;
        }
        else if (isdigit(current)) 
        {
            int value = 0;
            while (*m_index_ptr < f->length && isdigit(f->m_src[*m_index_ptr])) 
            {
                value = value * 10 + (f->m_src[*m_index_ptr] - '0');
                (*m_index_ptr)++;
            }

            tokens[Token_count].type = TOKEN_INT_LIT;
            tokens[Token_count].value = value;
            tokens[Token_count].line = Line_count;
            Token_count++;
        }
        else if (current == ';') 
        {
            tokens[Token_count].type = TOKEN_SEMI;
            tokens[Token_count].value = 0;
            tokens[Token_count].line = Line_count;
            Token_count++;
            consume(m_index_ptr);
        }
        else 
        {
            tokens[Token_count].type = TOKEN_INVALID;
            tokens[Token_count].value = 0;
            tokens[Token_count].line = Line_count;
            Token_count++;
            consume(m_index_ptr);
        }
    }
    
    // --- DEBUG CHECK 2 ---
    printf("DEBUG: Tokenize finished. Total tokens found = %d\n", Token_count);
    fflush(stdout);
}
