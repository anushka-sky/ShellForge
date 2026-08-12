#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/lexer.h"
#include "../include/token.h"

#define INPUT_SIZE 1024

int main(void)
{
    char input[INPUT_SIZE];

    printf("ShellForge\n");
    printf("Type 'exit' to quit.\n\n");

    while (1)
    {
        printf("shellforge$ ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        if (strlen(input) == 0)
        {
            continue;
        }

        TokenList list;

        token_list_init(&list);

        if (lexer_tokenize(input, &list) != 0)
        {
            token_list_free(&list);
            continue;
        }

        printf("\nTokens:\n");

        for (int i = 0; i < list.count; i++)
        {
            Token *token = &list.tokens[i];

            if (token->type == TOKEN_EOF)
            {
                printf("[%d] EOF\n", i);
            }
            else
            {
                printf("[%d] %-12s : %s\n",
                       i,
                       token_type_name(token->type),
                       token->value);
            }
        }

        printf("\n");

        token_list_free(&list);
    }

    printf("Exiting ShellForge...\n");

    return 0;
}
