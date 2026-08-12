#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/lexer.h"

static int is_operator(char c)
{
    return c == '|' ||
           c == '<' ||
           c == '>' ||
           c == '&';
}

static int is_unsupported(char c)
{
    return c == ';';
}

static void add_word(TokenList *list, char *buffer, int *length)
{
    if (*length > 0)
    {
        buffer[*length] = '\0';
        token_list_add(list, TOKEN_WORD, buffer);
        *length = 0;
    }
}

int lexer_tokenize(const char *input, TokenList *list)
{
    int i = 0;
    int length = 0;

    char buffer[1024];

    while (input[i] != '\0')
    {
        char c = input[i];

        /* Skip whitespace */
        if (isspace((unsigned char)c))
        {
            add_word(list, buffer, &length);
            i++;
            continue;
        }

        /* Unsupported semicolon */
        if (is_unsupported(c))
        {
            fprintf(stderr, "Lexer error: unsupported character ';'\n");
            token_list_add(list, TOKEN_ERROR, ";");
            return -1;
        }

        /* Single quoted string */
        if (c == '\'')
        {
            i++;

            while (input[i] != '\0' && input[i] != '\'')
            {
                if (length < 1023)
                {
                    buffer[length++] = input[i];
                }

                i++;
            }

            if (input[i] == '\0')
            {
                fprintf(stderr, "Lexer error: unmatched single quote\n");
                token_list_add(list, TOKEN_ERROR, "unmatched quote");
                return -1;
            }

            i++;
            continue;
        }

        /* Double quoted string */
        if (c == '"')
        {
            i++;

            while (input[i] != '\0' && input[i] != '"')
            {
                if (length < 1023)
                {
                    buffer[length++] = input[i];
                }

                i++;
            }

            if (input[i] == '\0')
            {
                fprintf(stderr, "Lexer error: unmatched double quote\n");
                token_list_add(list, TOKEN_ERROR, "unmatched quote");
                return -1;
            }

            i++;
            continue;
        }

        /* Operators */
        if (is_operator(c))
        {
            add_word(list, buffer, &length);

            if (c == '|')
            {
                token_list_add(list, TOKEN_PIPE, "|");
                i++;
            }
            else if (c == '<')
            {
                token_list_add(list, TOKEN_INPUT, "<");
                i++;
            }
            else if (c == '>')
            {
                if (input[i + 1] == '>')
                {
                    token_list_add(list, TOKEN_APPEND, ">>");
                    i += 2;
                }
                else
                {
                    token_list_add(list, TOKEN_OUTPUT, ">");
                    i++;
                }
            }
            else if (c == '&')
            {
                /*
                 * && is unsupported
                 */
                if (input[i + 1] == '&')
                {
                    fprintf(stderr,
                            "Lexer error: unsupported operator '&&'\n");

                    token_list_add(list, TOKEN_ERROR, "&&");
                    return -1;
                }

                token_list_add(list, TOKEN_BACKGROUND, "&");
                i++;
            }

            continue;
        }

        /*
         * Normal character.
         * Check for || before treating | as operator.
         */
        if (c == '|' && input[i + 1] == '|')
        {
            fprintf(stderr,
                    "Lexer error: unsupported operator '||'\n");

            token_list_add(list, TOKEN_ERROR, "||");
            return -1;
        }

        if (length < 1023)
        {
            buffer[length++] = c;
        }
        else
        {
            fprintf(stderr, "Lexer error: token too long\n");
            token_list_add(list, TOKEN_ERROR, "token too long");
            return -1;
        }

        i++;
    }

    add_word(list, buffer, &length);

    token_list_add(list, TOKEN_EOF, NULL);

    return 0;
}

