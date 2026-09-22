#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/lexer.h"
#include "../include/token.h"
#include "../include/history.h"
#include "../include/parser.h"
#include "../include/expand.h"
#include "../include/builtin.h"
#include "../include/executor.h"
#include <fcntl.h>
#include <unistd.h>

#define INPUT_SIZE 1024

int main(void)
{
    char input[INPUT_SIZE];

    History history;

    /* Initialize history */
    history_init(&history);

    /* Load previous commands */
    history_load(&history);

    printf("ShellForge\n");
    printf("Type 'exit' to quit.\n\n");

    while (1)
    {
        printf("shellforge$ ");
        fflush(stdout);

        /* Read command */
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        /* Remove newline */
        input[strcspn(input, "\n")] = '\0';

        /* Ignore empty input */
        if (strlen(input) == 0)
        {
            continue;
        }

        /* Exit */
        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        /* Show history */
        if (strcmp(input, "history") == 0)
        {
            history_print(&history);
            continue;
        }

        /* Add command to history */
        history_add(&history, input);

        /* ---------------- TOKENIZATION ---------------- */

        TokenList list;

        token_list_init(&list);

        if (lexer_tokenize(input, &list) != 0)
        {
            printf("Lexer error\n");
            token_list_free(&list);
            continue;
        }

        /* Display tokens */
        printf("\n");
        printf("============== TOKENS ================\n");

        for (int i = 0; i < list.count; i++)
        {
            Token *token = &list.tokens[i];

            if (token->type == TOKEN_EOF)
            {
                printf("%2d : END     END\n", i);
            }
            else
            {
                printf("%2d : %-7s %s\n",
                       i,
                       token_type_name(token->type),
                       token->value != NULL ? token->value : "");
            }
        }

        printf("=======================================\n");

        /* ---------------- PARSER ---------------- */

        Pipeline pipeline;

        if (parse_tokens(&list, &pipeline) != 0)
        {
            printf("Parser error\n");

            token_list_free(&list);
            continue;
        }

        /* ---------------- EXPAND / PIPELINE ---------------- */

        expand_pipeline(&pipeline);
/* ---------------- BUILTINS ---------------- */
/* ---------------- BUILTINS / EXTERNAL COMMANDS ---------------- */

int should_exit = 0;

for (int i = 0; i < pipeline.count; i++)
{
    Command *cmd = &pipeline.commands[i];
if (is_builtin(cmd))
{
    int saved_stdout = dup(STDOUT_FILENO);
    int output_fd = -1;

    if (cmd->output != NULL)
    {
        if (cmd->append)
        {
            output_fd = open(cmd->output,
                             O_WRONLY | O_CREAT | O_APPEND,
                             0644);
        }
        else
        {
            output_fd = open(cmd->output,
                             O_WRONLY | O_CREAT | O_TRUNC,
                             0644);
        }

        if (output_fd < 0)
        {
            perror("output");
            close(saved_stdout);
            continue;
        }

        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    int result = execute_builtin(cmd);

    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    if (strcmp(cmd->argv[0], "exit") == 0 && result == 1)
    {
        should_exit = 1;
    }
}
    else
    {
        execute_external(cmd);
    }
}

        /* Free pipeline */
        pipeline_free(&pipeline);

        /* Free tokens */
        token_list_free(&list);

        printf("\n");
        if (should_exit)
{
    break;
}
    }

    /* Exit message */
    printf("Exiting ShellForge...\n");

    /* Save history */
    history_save(&history);

    /* Free history */
    history_free(&history);

    return 0;
}
