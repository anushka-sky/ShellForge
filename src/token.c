#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/token.h"

void token_list_init(TokenList *list)
{
    list->count = 0;
    list->capacity = 10;

    list->tokens = malloc(sizeof(Token) * list->capacity);

    if (list->tokens == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
}

void token_list_add(TokenList *list, TokenType type, const char *value)
{
    if (list->count >= list->capacity)
    {
        list->capacity *= 2;

        Token *new_tokens =
            realloc(list->tokens, sizeof(Token) * list->capacity);

        if (new_tokens == NULL)
        {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        list->tokens = new_tokens;
    }

    list->tokens[list->count].type = type;

    if (value != NULL)
    {
        list->tokens[list->count].value = strdup(value);
    }
    else
    {
        list->tokens[list->count].value = NULL;
    }

    if (value != NULL && list->tokens[list->count].value == NULL)
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    list->count++;
}

void token_list_free(TokenList *list)
{
    for (int i = 0; i < list->count; i++)
    {
        free(list->tokens[i].value);
    }

    free(list->tokens);

    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

const char *token_type_name(TokenType type)
{
    switch (type)
    {
        case TOKEN_WORD:
            return "WORD";

        case TOKEN_PIPE:
            return "PIPE";

        case TOKEN_INPUT:
            return "INPUT";

        case TOKEN_OUTPUT:
            return "OUTPUT";

        case TOKEN_APPEND:
            return "APPEND";

        case TOKEN_BACKGROUND:
            return "BACKGROUND";

        case TOKEN_EOF:
            return "END";

        case TOKEN_ERROR:
            return "ERROR";

        default:
            return "UNKNOWN";
    }
}
