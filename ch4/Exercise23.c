
#include <pthread.h>
#include <stdio.h>
#include <math.h>

#include <stdlib.h>
#define PRIME 1
#define NOT_PRIME 0

int max;
void *primes(void *param);
int main(int argc, char const *argv[])
{
	if (argc - 1 == 0)
		return 0;
	max = atoi(argv[1]);

	pthread_t tid;
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);
	pthread_create(&tid, &attributes, primes, NULL);
	pthread_join(tid, NULL);
	return 0;
}

void *primes(void *param)
{

	if (max <= 0)
		return 0;
	if (max >= 1)
		printf("2\n");
	if (max >= 2)
		printf("3\n");
	int k = 1;
	int total = 2;
	int last = 3;
	while (total < max)
	{
		last++;
		if (last % 2 == 0)
			continue;
		if (last % 3 == 0)
			continue;
		int res = PRIME;
		for (int i = 1; (6 * i - 1) <= sqrt(last); i++)
		{
			if (last % (6 * i + 1) == 0 || last % (6 * i - 1) == 0)
				res = NOT_PRIME;
		}
		if (res == PRIME)
		{
			printf("%d\n", last);
			total++;
		}
	}

	pthread_exit(0);
}
