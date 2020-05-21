#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/time.h>
#include <string.h>
#define READ_END 0
#define WRITE_END 1

int main(int argc, char const *argv[])
{
    int fd[2];
    struct timeval start;
    struct timeval end;
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        close(fd[READ_END]);
        gettimeofday(&start, NULL);
        printf("start time: %ld\n", start.tv_usec);
        write(fd[WRITE_END], &start, sizeof(struct timeval));
        char file[25] = "/bin/";
        strcat(file, argv[1]);
        execlp(file, argv[1], NULL);
    }
    else
    {
        wait(NULL);
        gettimeofday(&end, NULL);
        close(fd[WRITE_END]);
        read(fd[READ_END], &start, sizeof(struct timeval));
        printf("start time: %ld\n", start.tv_usec);
        printf("%ld microseconds\n", end.tv_usec - start.tv_usec);
    }

    return 0;
}