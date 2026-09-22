#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "executor.h"

int execute_external(Command *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
    {
        return -1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {        /* Input redirection */

        if (cmd->input != NULL)
        {
            int fd = open(cmd->input, O_RDONLY);

            if (fd < 0)
            {
                perror("input");
                exit(EXIT_FAILURE);
            }

            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        /* Output redirection */

        if (cmd->output != NULL)
        {
            int fd;

            if (cmd->append)
            {
                fd = open(cmd->output,
                          O_WRONLY | O_CREAT | O_APPEND,
                          0644);
            }
            else
            {
                fd = open(cmd->output,
                          O_WRONLY | O_CREAT | O_TRUNC,
                          0644);
            }

            if (fd < 0)
            {
                perror("output");
                exit(EXIT_FAILURE);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(cmd->argv[0], cmd->argv);

        perror(cmd->argv[0]);
        exit(EXIT_FAILURE);
    }

    int status;

    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        return -1;
    }

    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }

    return -1;
}
