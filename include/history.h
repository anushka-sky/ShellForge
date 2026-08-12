#ifndef HISTORY_H
#define HISTORY_H

#define MAX_HISTORY 100

typedef struct
{
    char *commands[MAX_HISTORY];
    int count;
} History;

void history_init(History *history);
void history_add(History *history, const char *command);
void history_print(const History *history);
void history_free(History *history);

#endif
