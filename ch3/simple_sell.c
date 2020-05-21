#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#define MAXLINE 90 //maximum length command
#define OUTPUT 1
#define INPUT 0

int main(int argc, char const *argv[])
{
    char *args[MAXLINE / 2 + 1];
    char *history[MAXLINE / 2 + 1];
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
        //update history
        if (strcmp(args[0], "!!") != 0)
        {
            history_length = arg;
            for (int i = 0; i < arg; i++)
            {
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
            }
            else
                memcpy(args, history, sizeof(args));
        }
        //look for '<' or '>'
        for (int i = 0; i < arg; i++)
        {
            printf("%s \n", args[i]);
            if (args[i][0] == '>' || args[i][0] == '<')
            {
                if (strlen(args[i]) > 1)
                {
                    file = malloc(MAXLINE);
                    memcpy(file, &args[i][1], strlen(args[i]));
                    for (int j = i; j < arg - 1; j++)
                    {
                        args[j] = args[j + 1];
                    }
                }
                else
                {
                    file = args[i + 1];
                    for (int j = i + 1; j < arg - 1; j++)
                    {
                        args[j] = args[j + 1];
                    }
                }
                if (args[i][0] == '>')
                    redirect = OUTPUT;
                else
                    redirect = INPUT;
            }
        }
        if (file != NULL)
        {
            printf("%s\n", file);
        }

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
            execvp(args[0], args);
        }
        else
        {
            if (arg == 0 || args[arg - 1][0] != '&')
                wait(NULL);
        }
    }

    return 0;
}
