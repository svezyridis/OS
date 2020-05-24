#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#include <stdbool.h>
#define MAXLINE 90 //maximum length command
#define OUTPUT 1
#define INPUT 0
#define READ_END 0
#define WRITE_END 1

int main(int argc, char const *argv[])
{
    char *args[MAXLINE / 2 + 1];
    char *history[MAXLINE / 2 + 1] = {NULL};
    int history_length;
    char c;
    int should_run = 1;
    while (should_run)
    {
        printf("osh>");
        fflush(stdout);
        int arg = 0;
        int pt = 0;
        char *file = NULL;
        int redirect;
        bool syntax_error = false;
        args[0] = (char *)malloc(MAXLINE);
        for (int i = 0; i < MAXLINE; i++)
        {
            c = getc(stdin);
            if (c == EOF || c == '\n')
            {
                // if it ends with space dont increase index
                if (strlen(args[arg]) > 0)
                {
                    args[arg][pt] = '\0';
                    arg++;
                }
                args[arg] = NULL;
                break;
            }
            if (c == ' ' || c == '\t')
            {
                args[arg][pt] = '\0';
                arg++;
                pt = 0;
                args[arg] = (char *)malloc(MAXLINE);
            }
            else
            {
                args[arg][pt] = c;
                pt++;
            }
        }
        if (args[0] == NULL)
            continue;
        //update history
        if (strcmp(args[0], "!!") != 0)
        {
            history_length = arg;
            for (int i = 0; i < arg; i++)
            {
                if (history[i] == NULL)
                    history[i] = (char *)malloc(MAXLINE);
                for (int j = 0; j < strlen(args[i]); j++)
                {
                    history[i][j] = args[i][j];
                }
            }
            history[arg] = NULL;
        }
        else
        {
            if (history[0] == NULL)
            {
                printf("No commands in history\n");
                continue;
            }
            else
            {
                memcpy(args, history, sizeof(args));
                arg = history_length;
            }
        }
        //look for '<' or '>'
        for (int i = 0; i < arg; i++)
        {
            if (args[i][0] == '>' || args[i][0] == '<')
            {
                if (args[i][0] == '>')
                    redirect = OUTPUT;
                else
                    redirect = INPUT;
                if (strlen(args[i]) > 1)
                {
                    file = malloc(MAXLINE);
                    memcpy(file, &args[i][1], strlen(args[i]));
                    //remove file from args;
                    for (int j = i; j < arg; j++)
                    {
                        args[j] = args[j + 1];
                    }
                    arg--;
                }
                else
                {
                    if (args[i + 1] == NULL)
                    {
                        printf("syntax error near unexpected token `newline'\n");
                        syntax_error = true;
                        break;
                    }
                    file = malloc(MAXLINE);
                    strcpy(file, args[i + 1]);
                    for (int j = i; j < arg - 1; j++)
                    {
                        args[j] = args[j + 2];
                    }
                    args[arg - 1] = NULL;
                    arg -= 2;
                }
            }
        }
        if (syntax_error)
            continue;
        __pid_t pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "fork failed");
            exit(1);
        }
        else if (pid == 0)
        {
            if (arg >= 0 && args[arg - 1][0] == '&')
            {
                args[arg - 1] = NULL;
            }
            if (file != NULL)
            {
                if (redirect == OUTPUT)
                {
                    FILE *fp = fopen(file, "w");
                    int fd = fileno(fp);
                    dup2(fd, STDOUT_FILENO);
                }
                else
                {
                    FILE *fp = fopen(file, "r");
                    int fd = fileno(fp);
                    dup2(fd, STDIN_FILENO);
                }
            }
            char *second_command[MAXLINE / 2 + 1] = {NULL};
            for (int i = 0; i < arg; i++)
            {
                if (args[i][0] == '|')
                {
                    args[i] = NULL;
                    int j = 0;
                    while (args[j + i + 1] != NULL)
                    {
                        if (second_command[j] == NULL)
                            second_command[j] = (char *)malloc(MAXLINE);
                        strcpy(second_command[j], args[j + i + 1]);
                        args[j + i + 1] = NULL;
                        j++;
                    }
                    second_command[j] = NULL;
                    break;
                }
            }
            if (second_command[0] != NULL)
            {
                int fd[2];
                pipe(fd);
                pid = fork();
                if (pid < 0)
                {
                    fprintf(stderr, "fork failed");
                    exit(1);
                }
                else if (pid == 0)
                {
                    close(fd[READ_END]);
                    dup2(fd[WRITE_END], STDOUT_FILENO);
                    execvp(args[0], args);
                    printf("command %s not found\n", args[0]);
                }
                else
                {
                    wait(NULL);
                    close(fd[WRITE_END]);
                    dup2(fd[READ_END], STDIN_FILENO);
                    execvp(second_command[0], second_command);
                    printf("command %s not found\n", args[0]);
                }
            }
            execvp(args[0], args);
            printf("command %s not found\n", args[0]);
        }
        else
        {
            free(file);
            if (arg == 0 || args[arg - 1][0] != '&')
                wait(NULL);
        }
    }

    return 0;
}
