#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/time.h>
#include <string.h>
#include <sys/wait.h>

const char *name = "Collatz";
#define SIZE 4096 //arbitraty size that will not work if num will need more than 4095 iterations

int main(int argc, char const *argv[])
{

    if (argc != 2)
    {
        printf("please provide a postive integer arguement\n");
        exit(1);
    }
    int num = atoi(argv[1]);
    if (num <= 0)
    {
        printf("arguement must be a positive integer\n");
        exit(1);
    }
    int fd;
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE * sizeof(int));
    pid_t pid = fork();
    if (pid == 0)
    {
        int *ptr = (int *)
            mmap(0, SIZE * sizeof(int), PROT_WRITE, MAP_SHARED, fd, 0);
        *ptr = num;
        while (num != 1)
        {
            if (num % 2 == 1)
            {
                num = 3 * num + 1;
            }
            else
            {
                num = num / 2;
            }
            ptr++;
            *ptr = num;
        }
    }
    else
    {
        wait(NULL);
        int *ptr = (int *)
            mmap(0, SIZE * sizeof(int), PROT_READ, MAP_SHARED, fd, 0);
        while (*ptr != 1)
        {
            printf("%d\n", *ptr);
            ptr++;
        }
        printf("%d\n", *ptr);
    }

    return 0;
}