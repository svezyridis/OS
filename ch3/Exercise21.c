#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
    __pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        printf("fork failed\n");
        exit(1);
    }
    else if (pid == 0)
    {
        while (num != 1)
        {
            printf("%d \n", num);
            if (num % 2 == 1)
            {
                num = 3 * num + 1;
            }
            else
            {
                num = num / 2;
            }
        }
        printf("%d \n", num);
    }
    else
    {
        wait(NULL);
    }

    return 0;
}
