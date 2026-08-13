#pragma once

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "Base.h"

typedef enum 
{
    TOKEN_INT_LIT, // Number
    TOKEN_EXIT,    // exit
    TOKEN_SEMI, // ;
    TOKEN_EOF, // End Of File
  
    TOKEN_PLUS, // +
    TOKEN_MINUS, // -
    TOKEN_DIVIDE, // /
    TOKEN_MULTI, // *
    TOKEN_OPEN_PRAC, // (
    TOKEN_CLOSE_PRAC, // ) 
    TOKEN_EXPO, // ^
    
    TOKEN_INVALID  // Invalid
} TokenType;

typedef struct
{
    TokenType type;
    int value;
    int row;
    size_t col;
} Token;

size_t m_index = 0;
int Token_count = 0;
int Line_count = 1;
int col_count = 1;
Token *tokens = NULL;

void consume(size_t *index) { (*index)++; }
char peek(int offset, File *f) { return f->m_src[m_index + offset]; }

const char* token_type_to_string(TokenType type) 
{
    switch (type) 
    {
        case TOKEN_INT_LIT:    return "TOKEN_INT_LIT";
        case TOKEN_EXIT:       return "TOKEN_EXIT";
        case TOKEN_PLUS:       return "TOKEN_PLUS";
        case TOKEN_MINUS:      return "TOKEN_MINUS";
        case TOKEN_DIVIDE:     return "TOKEN_DIVIDE";
        case TOKEN_EXPO:       return "TOKEN_EXPO";
        case TOKEN_MULTI:      return "TOKEN_MULTI";
        case TOKEN_OPEN_PRAC:  return "TOKEN_OPEN_PRAC";
        case TOKEN_CLOSE_PRAC: return "TOKEN_CLOSE_PRAC";
        case TOKEN_SEMI:       return "TOKEN_SEMI";
        case TOKEN_EOF:        return "TOKEN_EOF";
        case TOKEN_INVALID:    return "TOKEN_INVALID";    
        default:               return "UNKNOWN";
    }
}

void add_token(TokenType type, int value, int row, size_t col)
{
    tokens[Token_count].type = type;
    tokens[Token_count].value = value;
    tokens[Token_count].row = row;
    tokens[Token_count].col = col;

    Token_count++;
}

void Tokenize(size_t *index, File *f)
{
    if (f == NULL) {
        printf("ERROR: File pointer 'f' is NULL!\n");
        return;
    }

    if (f->m_src == NULL) {
       printf("ERROR: f->m_src is NULL! Open_Read_File failed or couldn't find 'test.nv'.\n");
       return;
    }

    if (f->length <= 0) {
      printf("ERROR: File length is %zu (empty file or failed to read).\n"        ,f->length);
      return;
    }

    tokens = malloc(sizeof(Token) * (f->length + 1));

    if (tokens == NULL) {
       printf("ERROR: Failed to allocate memory for tokens!\n");
       return;
    }

    Token_count = 0;

    while (*index < f->length)
    {
        char current = f->m_src[*index];

        // Whitespace
        if (isspace((unsigned char)current))
        {
            if (current == '\n') {
                Line_count++;
                col_count = 1;
                consume(index);
            }
            else {
                consume(index);
                col_count++;
            }

            continue;
        }

        // Identifier / keyword
        if (isalpha((unsigned char)current))
        {
            // Save where this token STARTS
            size_t token_col = col_count;

            char buffer[256];
            int buffer_index = 0;

            while (*index < f->length &&
                   isalpha((unsigned char)f->m_src[*index]))
            {
                if (buffer_index < 255) {
                    buffer[buffer_index++] = f->m_src[*index];
                }

                (*index)++;
                col_count++;
            }

            buffer[buffer_index] = '\0';

            if (strcmp(buffer, "exit") == 0)
            {
                tokens[Token_count].type = TOKEN_EXIT;
                tokens[Token_count].value = 0;
            }
            else
            {
                tokens[Token_count].type = TOKEN_INVALID;
                tokens[Token_count].value = 0;
            }

            tokens[Token_count].row = Line_count;
            tokens[Token_count].col = token_col;

            Token_count++;
        }

        // Integer literal
        else if (isdigit((unsigned char)current))
        {
            // Save where the number STARTS
            size_t token_col = col_count;

            int value = 0;

            while (*index < f->length &&
                   isdigit((unsigned char)f->m_src[*index]))
            {
                value = value * 10 +
                        (f->m_src[*index] - '0');

                (*index)++;
                col_count++;
            }

            add_token(TOKEN_INT_LIT, value, Line_count, token_col);
        }
        // Plus
        else if (current == '+') {
            add_token(TOKEN_PLUS, 0, Line_count, col_count); 
            consume(index);
            col_count++;
        }
        // Minus
        else if (current == '-') {
            add_token(TOKEN_MINUS, 0, Line_count, col_count); 
            consume(index);
            col_count++;
        }

        // Comments
        else if (*index + 1 < f->length &&
            f->m_src[*index] == '/' &&
            f->m_src[*index + 1] == '/'){
    
            *index += 2;
            col_count += 2;

            while (*index < f->length && f->m_src[*index] != '\n')
            {
                (*index)++;
                col_count++;
            }

            continue;
        }
       
        // Divide
        else if (current == '/') {
            add_token(TOKEN_DIVIDE, 0, Line_count, col_count); 
            consume(index);
            col_count++;
        }
        
        // Expo
        else if (current == '^') {
            add_token(TOKEN_EXPO, 0, Line_count, col_count); 
            consume(index);
            col_count++;
        }
        // Multiply
        else if (current == '*') {
            add_token(TOKEN_MULTI, 0, Line_count, col_count); 
            consume(index);
            col_count++;
        }
        // Open Prac
        else if (current == '(') {
            add_token(TOKEN_OPEN_PRAC, 0, Line_count, col_count); 
            consume(index);
            col_count++;
        }
        // Close Prac
        else if (current == ')') {
            add_token(TOKEN_CLOSE_PRAC, 0, Line_count, col_count); 
            consume(index);
            col_count++;
        }
        
        // End Of File
        else if (*index >= f->length) {
            add_token(TOKEN_EOF, 0, Line_count, col_count);
        }
        
        // Semicolon
        else if (current == ';')
        {
            add_token(TOKEN_SEMI, 0, Line_count, col_count);
            consume(index);
            col_count++;
        }
     
        // Anything else
        else
        {
            add_token(TOKEN_INVALID, 0, Line_count, col_count);
            consume(index);
            col_count++;
        }
    }

    if (Debug == true) {
        printf("DEBUG: Tokenize finished. Total tokens found = %d\n",
        Token_count);   
    }
}
