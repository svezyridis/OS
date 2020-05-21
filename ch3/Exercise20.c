#include <stdlib.h>
#include <stdio.h>
#define MIN_PID 300
#define MAX_PID 5000

short *pids;
int allocate_map(void)
{
    pids = (short *)calloc((MAX_PID - MIN_PID + 1), sizeof(short));
    if (pids == NULL)
        return -1;
    for (int i = 300; i <= 5000; i++)
    {
        pids[i - 300] = 0;
    }
    return 1;
}

int allocate_pid(void)
{
    for (int i = MIN_PID; i <= MAX_PID; i++)
    {
        if (pids[i - MIN_PID] == 0)
        {
            pids[i - MIN_PID] = 1;
            return i;
        }
    }

    return -1;
}

void release_pid(int pid)
{
    pids[pid - MIN_PID] = 0;
}

int main(int argc, char const *argv[])
{
    if (allocate_map() == -1)
    {
        printf("map could not be allocated\n");
        exit(1);
    }
    for (int i = 0; i < 6; i++)
    {
        int pid = allocate_pid();
        if (pid == -1)
        {
            printf("id could not be allocated\n");
        }
        else
        {
            printf("id: %d\n", pid);
        }
    }

    return 0;
}
