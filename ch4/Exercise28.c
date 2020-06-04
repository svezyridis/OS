#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define MIN_PID 300
#define MAX_PID 5000

short *pids;
int no_of_threads;
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

void *wait_random(void *params);

int main(int argc, char const *argv[])
{
	if (argc - 1 == 0)
		return 0;
	no_of_threads = atoi(argv[1]);
	if (allocate_map() == -1)
	{
		printf("map could not be allocated\n");
		exit(1);
	}
	pthread_t workers[no_of_threads];
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	for (int i = 0; i < no_of_threads; i++)
	{
		pthread_create(&workers[i], &attributes, wait_random, NULL);
	}
	for (int i = 0; i < no_of_threads; i++)
	{
		pthread_join(workers[i], NULL);
	}

	return 0;
}

void *wait_random(void *params)
{
	int tid = allocate_pid();
	if (tid == -1)
	{
		printf("allocation failed\n");
		pthread_exit(0);
	}
	else
	{
		printf("id=%d\n", tid);
	}

	sleep(rand() % 5);
	pthread_exit(0);
}