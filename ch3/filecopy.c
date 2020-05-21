#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <wait.h>
#define READ_END 0
#define WRITE_END 1

int main(int argc, char const *argv[])
{
    int fd[2];
    int c;
    size_t n = 0;
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        close(fd[WRITE_END]);
        FILE *fp;
        fp = fopen(argv[2], "w");
        if (fp == NULL)
        {
            fprintf(stderr, "could not open file for writing");
            exit(1);
        }
        int size;
        //read size
        read(fd[READ_END], &size, sizeof(int));
        //read characters
        char file[size];
        read(fd[READ_END], file, size);
        fputs(file, fp);
    }
    else
    {
        close(fd[READ_END]);
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            fprintf(stderr, "could not open the file for reading");
            exit(1);
        }
        char *contents;
        fseek(fp, 0, SEEK_END); // seek to end of file
        int size = ftell(fp);   // get current file pointer
        contents = malloc(size + 1);
        fseek(fp, 0, SEEK_SET);

        while ((c = fgetc(fp)) != EOF)
        {
            contents[n++] = (char)c;
        }
        //terminate with the null character
        contents[n] = '\0';
        //signal array size
        write(fd[WRITE_END], &size, sizeof(int));
        //write the characters
        write(fd[WRITE_END], contents, size + 1);
    }

    return 0;
}