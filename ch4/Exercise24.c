#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <sys/random.h>
#include <stdint.h>
#include <time.h>

#include <stdlib.h>

int total_points, circle_points = 0;
void *generate_points(void *param);
int main(int argc, char const *argv[])
{
	if (argc - 1 == 0)
		return 0;
	total_points = atoi(argv[1]);

	pthread_t tid;
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);
	pthread_create(&tid, &attributes, generate_points, NULL);
	pthread_join(tid, NULL);
	double pi = 4 * (double)circle_points / (double)total_points;
	printf("pi=%f\n", pi);
	return 0;
}

void *generate_points(void *param)
{
	// ...
	srand(time(NULL));
	for (size_t i = 0; i < total_points; i++)
	{
		double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
		double y = (double)rand() / RAND_MAX * 2.0 - 1.0;
		double distance = sqrt(x * x + y * y);
		if (distance <= 1)
			circle_points++;
	}
}
