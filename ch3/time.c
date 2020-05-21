#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/time.h>
#include <string.h>
const char *name = "OS";

int main(int argc, char const *argv[])
{
    int fd;
    struct timeval start;
    struct timeval end;
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(struct timeval));
    pid_t pid = fork();
    if (pid == 0)
    {
        struct timeval *ptr = (struct timeval *)
            mmap(0, sizeof(struct timeval), PROT_WRITE, MAP_SHARED, fd, 0);
        gettimeofday(&start, NULL);
        *ptr = start;
        char file[25] = "/bin/";
        strcat(file, argv[1]);
        execlp(file, argv[1], NULL);
    }
    else
    {
        wait(NULL);
        gettimeofday(&end, NULL);
        struct timeval *ptr = (struct timeval *)
            mmap(0, sizeof(struct timeval), PROT_READ, MAP_SHARED, fd, 0);
        printf("%ld microseconds\n", end.tv_usec - ptr->tv_usec);
        shm_unlink(name);
    }

    return 0;
}