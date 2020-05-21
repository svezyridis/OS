#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <wait.h>
#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 32

int main(int argc, char const *argv[])
{
    int fd1[2];
    int fd2[2];
    if (pipe(fd1) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    if (pipe(fd2) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        close(fd1[WRITE_END]);
        close(fd2[READ_END]);
        char message[BUFFER_SIZE];
        read(fd1[READ_END], message, BUFFER_SIZE);
        printf("child : message: %s\n", message);
        for (int i = 0; i < strlen(message) + 1; i++)
        {
            if (islower(message[i]))
                message[i] = toupper(message[i]);
            else
            {
                message[i] = tolower(message[i]);
            }
        }
        write(fd2[WRITE_END], message, strlen(message) + 1);
    }
    else
    {
        close(fd2[WRITE_END]);
        close(fd1[READ_END]);
        const char *message = "thiS is the MeSsage";
        write(fd1[WRITE_END], message, strlen(message) + 1);
        wait(NULL);
        char response[BUFFER_SIZE];
        read(fd2[READ_END], response, strlen(message) + 1);
        printf("parent: reversed message: %s\n", response);
    }

    return 0;
}