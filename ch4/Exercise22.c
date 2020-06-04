
#include <pthread.h>
#include <stdio.h>

#include <stdlib.h>
#define NUM_THREADS 3

int size, min, max;
int *array;
double average;
void *calc_avg(void *param);
void *calc_min(void *param);
void *calc_max(void *param);
int main(int argc, char const *argv[])
{
	size = argc - 1;
	printf("%d\n", size);
	if (size == 0)
		return 0;
	array = malloc(size * sizeof(int));
	for (size_t i = 0; i < size; i++)
	{
		array[i] = atoi(argv[i + 1]);
	}

	pthread_t workers[NUM_THREADS];
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);
	pthread_create(&workers[0], &attributes, calc_avg, NULL);
	pthread_create(&workers[1], &attributes, calc_max, NULL);
	pthread_create(&workers[2], &attributes, calc_min, NULL);

	for (int i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(workers[i], NULL);
	}
	printf("average=%.2f min=%d max=%d\n", average, min, max);
	free(array);
	/* code */
	return 0;
}

void *calc_max(void *param)
{

	max = array[0];
	for (int i = 1; i < size; i++)
	{
		if (array[i] > max)
			max = array[i];
	}
	pthread_exit(0);
}
void *calc_min(void *param)
{

	min = array[0];
	for (int i = 1; i < size; i++)
	{
		if (array[i] < min)
			min = array[i];
	}
	pthread_exit(0);
}
void *calc_avg(void *param)
{

	int sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum += array[i];
	}
	average = (double)sum / (double)size;
	pthread_exit(0);
}
