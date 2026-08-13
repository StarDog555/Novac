#pragma once
#include "./lexer.h"

typedef enum
{
    EXPE_INT,
    EXPE_PLUS,
    EXPE_MINUS,
    EXPE_MULTI,
    EXPE_DIVIDE,
    EXPE_EXPO
} EXPE_Type;

typedef struct EXPE
{
    EXPE_Type Type;

    int value;

    struct EXPE *left;
    struct EXPE *right;
} EXPE;


typedef struct
{
    EXPE *E;
} EXIT;


typedef struct
{
    EXIT *Exit;
} Part;

typedef struct
{
    Part *part;
    size_t Part_count;
} _Root;

EXIT *temp;

void Print_EXPE(EXPE *e, int depth)
{
    if (e == NULL)
        return;

    for (int i = 0; i < depth; i++)
        printf("  ");

    switch (e->Type)
    {
        case EXPE_INT:
            printf("INT: %d\n", e->value);
            break;

        case EXPE_PLUS:
            printf("PLUS\n");
            break;

        case EXPE_MINUS:
            printf("MINUS\n");
            break;

        case EXPE_MULTI:
            printf("MULTI\n");
            break;

        case EXPE_DIVIDE:
            printf("DIVIDE\n");
            break;

        case EXPE_EXPO:
            printf("EXPO\n");
            break;
    }

    Print_EXPE(e->left, depth + 1);
    Print_EXPE(e->right, depth + 1);
}

EXPE *Parse_EXPE(Token *tokens, size_t *index, int min_precedence)
{
    Token *token = &tokens[*index];

    EXPE *left = NULL;

    if (token->type == TOKEN_INT_LIT)
    {
        left = malloc(sizeof(EXPE));

        if (left == NULL)
        {
            Exit_With_Error("Failed to allocate EXPE", NULL, NULL);
        }

        left->Type = EXPE_INT;
        left->value = token->value;
        left->left = NULL;
        left->right = NULL;

        consume(index);
    }
    else if (token->type == TOKEN_OPEN_PRAC)
    {
        consume(index); // (

        left = Parse_EXPE(tokens, index, 0);

        if (tokens[*index].type != TOKEN_CLOSE_PRAC)
        {
            char tempmsg[2056];

            snprintf(
                tempmsg,
                sizeof(tempmsg),
                "Expected ')' %d:%zu",
                tokens[*index].row,
                tokens[*index].col
            );

            Exit_With_Error(tempmsg, NULL, NULL);
        }

        consume(index); // )
    }
    else
    {
        char tempmsg[2056];

        snprintf(
            tempmsg,
            sizeof(tempmsg),
            "Expected expression %d:%zu",
            token->row,
            token->col
        );

        Exit_With_Error(tempmsg, NULL, NULL);
    }

    while (1)
    {
        TokenType op = tokens[*index].type;
        int precedence;

        switch (op)
        {
            case TOKEN_PLUS:
            case TOKEN_MINUS:
                precedence = 1;
                break;

            case TOKEN_MULTI:
            case TOKEN_DIVIDE:
                precedence = 2;
                break;

            case TOKEN_EXPO:
                precedence = 3;
                break;

            default:
                return left;
        }

        if (precedence < min_precedence)
        {
            return left;
        }

        consume(index);

        /*
         * ^ should eventually be right-associative.
         * For now, use precedence + 1 for left-associative
         * operators.
         */
        int next_precedence = precedence + 1;

        EXPE *right = Parse_EXPE(
            tokens,
            index,
            next_precedence
        );

        EXPE *e = malloc(sizeof(EXPE));

        if (e == NULL)
        {
            Exit_With_Error("Failed to allocate EXPE", NULL, NULL);
        }

        if (op == TOKEN_PLUS)
            e->Type = EXPE_PLUS;
        else if (op == TOKEN_MINUS)
            e->Type = EXPE_MINUS;
        else if (op == TOKEN_MULTI)
            e->Type = EXPE_MULTI;
        else if (op == TOKEN_DIVIDE)
            e->Type = EXPE_DIVIDE;
        else if (op == TOKEN_EXPO)
            e->Type = EXPE_EXPO;

        e->value = 0;
        e->left = left;
        e->right = right;

        left = e;
    }
}

EXIT *Parse_EXIT(Token *tokens, size_t *index)
{
    EXIT *exit_node = malloc(sizeof(EXIT));

    if (exit_node == NULL)
    {
        Exit_With_Error("Failed to allocate exit_node", NULL, NULL);
    }

    consume(index); // TOKEN_EXIT
    exit_node->E = Parse_EXPE(tokens, index, 0);

    temp = exit_node;
    
    if (tokens[*index].type != TOKEN_SEMI)
    {
        Exit_With_Error(
            "Expected ';' after exit EXPE",
            NULL,
            NULL
        );
    }
    
    consume(index); // TOKEN_SEMI
    return exit_node;
}

_Root *Parse(Token *tokens, size_t Token_count)
{
    if (tokens == NULL){
        Exit_With_Error("Tokens is NULL", NULL, NULL);
    }

    _Root *root = malloc(sizeof(_Root));

    if (root == NULL){
        Exit_With_Error("Failed to allocate _Root", NULL, NULL);
    }

    root->part = NULL;
    root->Part_count = 0;

    size_t index = 0;

    while (index < Token_count &&
           tokens[index].type != TOKEN_EOF)
    {
        Token *token = &tokens[index];

        if (token->type == TOKEN_EXIT){
            Parse_EXIT(tokens, &index);
        }
        else{
            Compiled = false;
            Exit_With_Error("Unexpected Token", NULL, NULL);
        }
    }

    return root;
}
