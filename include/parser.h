#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#define MAX_COMMANDS 32
#define MAX_ARGUMENTS 64

typedef struct
{
    char *argv[MAX_ARGUMENTS];
    int argc;

    char *input;
    char *output;

    int append;
    int background;

} Command;

typedef struct
{
    Command commands[MAX_COMMANDS];
    int count;

} Pipeline;

int parse_tokens(TokenList *tokens, Pipeline *pipeline);

void pipeline_free(Pipeline *pipeline);

#endif
