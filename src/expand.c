#include <stdio.h>

#include "../include/expand.h"

void expand_pipeline(Pipeline *pipeline)
{
    if (pipeline == NULL)
    {
        return;
    }

    printf("\n");
    printf("============== PIPELINE ==============\n");

    for (int i = 0; i < pipeline->count; i++)
    {
        Command *command = &pipeline->commands[i];

        printf("\n");
        printf("Command %d\n", i + 1);
        printf("------------------------\n");

        printf("Arguments\n");

        for (int j = 0; j < command->argc; j++)
        {
            printf("argv[%d] = %s\n",
                   j,
                   command->argv[j]);
        }

        printf("Input      : %s\n",
               command->input != NULL
                   ? command->input
                   : "None");

        printf("Output     : %s\n",
               command->output != NULL
                   ? command->output
                   : "None");

        printf("Append     : %s\n",
               command->append ? "Yes" : "No");

        printf("Background : %s\n",
               command->background ? "Yes" : "No");
    }

    printf("\n");
    printf("=======================================\n");
}
