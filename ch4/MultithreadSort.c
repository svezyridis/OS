#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

int *array;
int *aux;
int length;
typedef struct
{
	int start_index;
	int end_index;
} parameters;
void *sort(void *params);
void *merge(void *params);
_Bool is_sorted(int *array, int length);

int main(int argc, char const *argv[])
{
	if (argc - 1 == 0)
		return 0;
	length = atoi(argv[1]);
	srand(time(NULL));
	array = malloc(length * sizeof(*array));
	printf("initial array:");
	for (size_t i = 0; i < length; i++)
	{
		array[i] = rand() % 100;
		printf(" %d", array[i]);
	}
	printf("\n");
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);
	pthread_t sort1;
	pthread_t sort2;
	pthread_t merge_id;
	parameters *data1 = malloc(sizeof(*data1));
	data1->start_index = 0;
	data1->end_index = length / 2;
	pthread_create(&sort1, &attributes, sort, data1);
	parameters *data2 = malloc(sizeof(*data1));
	data2->start_index = length / 2;
	data2->end_index = length;
	pthread_create(&sort2, &attributes, sort, data2);
	pthread_join(sort1, NULL);
	free(data1);
	pthread_join(sort2, NULL);
	free(data2);
	printf("array before merge");
	for (size_t i = 0; i < length; i++)
	{
		printf(" %d", array[i]);
	}
	printf("\n");

	pthread_create(&merge_id, &attributes, merge, NULL);
	pthread_join(merge_id, NULL);
	if (!is_sorted(array, length))
	{
		printf("sorted failed\n");
		exit(0);
	}
	printf("sorted array:");
	for (size_t i = 0; i < length; i++)
	{
		printf(" %d", array[i]);
	}
	printf("\n");

	return 0;
}

_Bool is_sorted(int *array, int length)
{
	for (size_t i = 0; i < length - 1; i++)
	{
		if (array[i + 1] < array[i])
			return 0;
	}
	return 1;
}

void *sort(void *params)
{
	parameters *ptr = params;
	int start = ptr->start_index;
	int end = ptr->end_index;
	//insertion sort
	for (size_t i = start + 1; i < end; i++)
	{
		for (size_t j = i; j > start && array[j] < array[j - 1]; j--)
		{
			int temp = array[j];
			array[j] = array[j - 1];
			array[j - 1] = temp;
		}
	}
	pthread_exit(0);
}
void *merge(void *params)
{
	aux = malloc(length * sizeof(*aux));
	memcpy(aux, array, length * sizeof(*array));
	int mid = (length - 1) / 2;
	int i = 0;
	int j = mid + 1;
	for (size_t k = 0; k < length; k++)
	{
		if (i > mid)
			array[k] = aux[j++];
		else if (j > length - 1)
			array[k] = aux[i++];
		else if (aux[j] > aux[i])
			array[k] = aux[i++];
		else
			array[k] = aux[j++];
	}
	pthread_exit(0);
}
