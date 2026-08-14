#pragma once
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "Base.h"

typedef enum {
    TOKEN_INT_LIT,
    TOKEN_EXIT,
    TOKEN_LET,
    TOKEN_EQ,
    TOKEN_IDENTIFIER,
    TOKEN_SEMI,
    TOKEN_EOF,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_DIVIDE,
    TOKEN_MULTI,
    TOKEN_OPEN_PRAC,
    TOKEN_CLOSE_PRAC,
    TOKEN_EXPO,
    TOKEN_INVALID
} TokenType;

typedef struct {
    char **Words;
} KEYWORDS;

typedef struct {
    TokenType type;
    int value;
    char *String;
    int row;
    size_t col;
} Token;

size_t m_index = 0;
int Token_count = 0;
int Line_count = 1;
int col_count = 1;
Token *tokens = NULL;

void consume(size_t *index) {
    (*index)++;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_INT_LIT: return "TOKEN_INT_LIT";
        case TOKEN_EXIT: return "TOKEN_EXIT";
        case TOKEN_PLUS: return "TOKEN_PLUS";
        case TOKEN_MINUS: return "TOKEN_MINUS";
        case TOKEN_DIVIDE: return "TOKEN_DIVIDE";
        case TOKEN_EXPO: return "TOKEN_EXPO";
        case TOKEN_MULTI: return "TOKEN_MULTI";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_LET: return "TOKEN_LET";
        case TOKEN_EQ: return "TOKEN_EQ";
        case TOKEN_OPEN_PRAC: return "TOKEN_OPEN_PRAC";
        case TOKEN_CLOSE_PRAC: return "TOKEN_CLOSE_PRAC";
        case TOKEN_SEMI: return "TOKEN_SEMI";
        case TOKEN_EOF: return "TOKEN_EOF";
        case TOKEN_INVALID: return "TOKEN_INVALID";
        default: return "UNKNOWN";
    }
}

void add_token(TokenType type, int value, int row, size_t col) {
    tokens[Token_count].type = type;
    tokens[Token_count].value = value;
    tokens[Token_count].String = NULL;
    tokens[Token_count].row = row;
    tokens[Token_count].col = col;
    Token_count++;
}

void Tokenize(size_t *index, File *f) {
    if (index == NULL) {
        printf("ERROR: Tokenize received NULL index!\n");
        return;
    }

    if (f == NULL) {
        printf("ERROR: File pointer 'f' is NULL!\n");
        return;
    }

    if (f->m_src == NULL) {
        printf("ERROR: f->m_src is NULL!\n");
        return;
    }

    if (f->length <= 0) {
        printf("ERROR: File is empty!\n");
        return;
    }

    tokens = malloc(sizeof(Token) * (f->length + 1));

    if (tokens == NULL) {
        printf("ERROR: Failed to allocate memory for tokens!\n");
        return;
    }

    Token_count = 0;
    Line_count = 1;
    col_count = 1;

    while (*index < f->length) {
        char current = f->m_src[*index];

        if (isspace((unsigned char)current)) {
            if (current == '\n') {
                Line_count++;
                col_count = 1;
            } else {
                col_count++;
            }

            consume(index);
            continue;
        }

        if (isalpha((unsigned char)current) || current == '_') {
            size_t token_col = col_count;
            char buffer[256];
            int buffer_index = 0;

            while (*index < f->length &&
                   (isalpha((unsigned char)f->m_src[*index]) ||
                    isdigit((unsigned char)f->m_src[*index]) ||
                    f->m_src[*index] == '_')) {

                if (buffer_index < 255)
                    buffer[buffer_index++] = f->m_src[*index];

                (*index)++;
                col_count++;
            }

            buffer[buffer_index] = '\0';

            TokenType type;

            if (strcmp(buffer, "exit") == 0)
                type = TOKEN_EXIT;
            else if (strcmp(buffer, "let") == 0)
                type = TOKEN_LET;
            else
                type = TOKEN_IDENTIFIER;

            add_token(type, 0, Line_count, token_col);

            if (type == TOKEN_IDENTIFIER) {
                tokens[Token_count - 1].String =
                    malloc(strlen(buffer) + 1);

                if (tokens[Token_count - 1].String == NULL) {
                    printf("ERROR: Failed to allocate identifier string!\n");
                    return;
                }

                strcpy(tokens[Token_count - 1].String, buffer);
            }

            continue;
        }

        if (isdigit((unsigned char)current)) {
            size_t token_col = col_count;
            int value = 0;

            while (*index < f->length &&
                   isdigit((unsigned char)f->m_src[*index])) {

                value = value * 10 +
                        (f->m_src[*index] - '0');

                (*index)++;
                col_count++;
            }

            add_token(
                TOKEN_INT_LIT,
                value,
                Line_count,
                token_col
            );

            continue;
        }

        if (*index + 1 < f->length &&
            f->m_src[*index] == '/' &&
            f->m_src[*index + 1] == '/') {

            *index += 2;
            col_count += 2;

            while (*index < f->length &&
                   f->m_src[*index] != '\n') {

                (*index)++;
                col_count++;
            }

            continue;
        }

        if (current == '+') {
            add_token(TOKEN_PLUS, 0, Line_count, col_count);
            consume(index);
            col_count++;
            continue;
        }

        if (current == '-') {
            add_token(TOKEN_MINUS, 0, Line_count, col_count);
            consume(index);
            col_count++;
            continue;
        }

        if (current == '/') {
            add_token(TOKEN_DIVIDE, 0, Line_count, col_count);
            consume(index);
            col_count++;
            continue;
        }

        if (current == '*') {
            add_token(TOKEN_MULTI, 0, Line_count, col_count);
            consume(index);
            col_count++;
            continue;
        }

        if (current == '^') {
            add_token(TOKEN_EXPO, 0, Line_count, col_count);
            consume(index);
            col_count++;
            continue;
        }

        if (current == '(') {
            add_token(TOKEN_OPEN_PRAC, 0, Line_count, col_count);
            consume(index);
            col_count++;
            continue;
        }

        if (current == ')') {
            add_token(TOKEN_CLOSE_PRAC, 0, Line_count, col_count);
            consume(index);
            col_count++;
            continue;
        }

        if (current == '=') {
            add_token(TOKEN_EQ, 0, Line_count, col_count);
            consume(index);
            col_count++;
            continue;
        }

        if (current == ';') {
            add_token(TOKEN_SEMI, 0, Line_count, col_count);
            consume(index);
            col_count++;
            continue;
        }

        printf(
            "ERROR: Invalid character '%c' at %d:%d\n",
            current,
            Line_count,
            col_count
        );

        add_token(
            TOKEN_INVALID,
            0,
            Line_count,
            col_count
        );

        consume(index);
        col_count++;
    }

    add_token(
        TOKEN_EOF,
        0,
        Line_count,
        col_count
    );

    if (Debug == true) {
        printf(
            "DEBUG: Tokenize finished. Total tokens found = %d\n",
            Token_count
        );

        for (int i = 0; i < Token_count; i++) {
            printf(
                "[%d] %s",
                i,
                token_type_to_string(tokens[i].type)
            );

            if (tokens[i].type == TOKEN_INT_LIT)
                printf(" (%d)", tokens[i].value);

            if (tokens[i].type == TOKEN_IDENTIFIER &&
                tokens[i].String != NULL)
                printf(" (%s)", tokens[i].String);

            printf(
                " at %d:%zu\n",
                tokens[i].row,
                tokens[i].col
            );
        }
    }
}
