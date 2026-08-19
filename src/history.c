#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/history.h"

void history_init(History *history)
{
    history->count = 0;

    for (int i = 0; i < MAX_HISTORY; i++)
    {
        history->commands[i] = NULL;
    }
}

void history_add(History *history, const char *command)
{
    if (command == NULL || strlen(command) == 0)
    {
        return;
    }

    /* If history is full, remove the oldest command */
    if (history->count >= MAX_HISTORY)
    {
        free(history->commands[0]);

        for (int i = 1; i < MAX_HISTORY; i++)
        {
            history->commands[i - 1] = history->commands[i];
        }

        history->count = MAX_HISTORY - 1;
    }

    history->commands[history->count] = strdup(command);

    if (history->commands[history->count] == NULL)
    {
        perror("strdup");
        return;
    }

    history->count++;
}

void history_print(const History *history)
{
    for (int i = 0; i < history->count; i++)
    {
        printf("%d  %s\n", i + 1, history->commands[i]);
    }
}

void history_load(History *history)
{
    FILE *file = fopen(HISTORY_FILE, "r");

    /* History file doesn't exist yet */
    if (file == NULL)
    {
        return;
    }

    char line[1024];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) > 0)
        {
            history_add(history, line);
        }
    }

    fclose(file);
}

void history_save(const History *history)
{
    FILE *file = fopen(HISTORY_FILE, "w");

    if (file == NULL)
    {
        perror("history");
        return;
    }

    for (int i = 0; i < history->count; i++)
    {
        fprintf(file, "%s\n", history->commands[i]);
    }

    fclose(file);
}

void history_free(History *history)
{
    for (int i = 0; i < history->count; i++)
    {
        free(history->commands[i]);
        history->commands[i] = NULL;
    }

    history->count = 0;
}
