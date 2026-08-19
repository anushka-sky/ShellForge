#include <stdio.h>
#include <string.h>

#include "../include/parser.h"

int parse_tokens(TokenList *tokens, Pipeline *pipeline)
{
    if (tokens == NULL || pipeline == NULL)
    {
        return -1;
    }

    pipeline->count = 0;

    Command *command = &pipeline->commands[0];

    memset(command, 0, sizeof(Command));

    pipeline->count = 1;

    for (int i = 0; i < tokens->count; i++)
    {
        Token *token = &tokens->tokens[i];

        /* End of input */
        if (token->type == TOKEN_EOF)
        {
            break;
        }

        /* Pipe */
        if (token->type == TOKEN_PIPE)
        {
            if (pipeline->count >= MAX_COMMANDS)
            {
                printf("Error: too many commands in pipeline\n");
                return -1;
            }

            pipeline->count++;

            command = &pipeline->commands[pipeline->count - 1];

            memset(command, 0, sizeof(Command));

            continue;
        }

        /* Input redirection < */
        if (token->type == TOKEN_INPUT)
        {
            if (i + 1 < tokens->count)
            {
                i++;
                command->input = tokens->tokens[i].value;
            }

            continue;
        }

        /* Output redirection > */
        if (token->type == TOKEN_OUTPUT)
        {
            if (i + 1 < tokens->count)
            {
                i++;
                command->output = tokens->tokens[i].value;
            }

            command->append = 0;

            continue;
        }

        /* Append redirection >> */
        if (token->type == TOKEN_APPEND)
        {
            if (i + 1 < tokens->count)
            {
                i++;
                command->output = tokens->tokens[i].value;
            }

            command->append = 1;

            continue;
        }

        /* Background & */
        if (token->type == TOKEN_BACKGROUND)
        {
            command->background = 1;
            continue;
        }

        /* Normal argument */
        if (token->type == TOKEN_WORD)
        {
            if (command->argc < MAX_ARGUMENTS - 1)
            {
                command->argv[command->argc] = token->value;
                command->argc++;

                command->argv[command->argc] = NULL;
            }
        }
    }

    return 0;
}

void pipeline_free(Pipeline *pipeline)
{
    if (pipeline == NULL)
    {
        return;
    }

    pipeline->count = 0;
}
