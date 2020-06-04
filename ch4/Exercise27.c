#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <sys/random.h>
#include <stdint.h>
#include <time.h>

#include <stdlib.h>

int numbers;
long *sequence;
void *generate_numbers(void *param);
int main(int argc, char const *argv[])
{
	if (argc - 1 == 0)
		return 0;
	numbers = atoi(argv[1]);
	sequence = malloc(numbers * sizeof(*sequence));
	pthread_t tid;
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);
	pthread_create(&tid, &attributes, generate_numbers, NULL);
	pthread_join(tid, NULL);
	for (size_t i = 0; i < numbers; i++)
	{
		printf("%ld\n", sequence[i]);
	}

	return 0;
}

void *generate_numbers(void *param)
{
	if (numbers >= 1)
		sequence[0] = 0;
	if (numbers >= 2)
		sequence[1] = 1;
	for (size_t i = 2; i < numbers; i++)
	{
		sequence[i] = sequence[i - 1] + sequence[i - 2];
	}
}
