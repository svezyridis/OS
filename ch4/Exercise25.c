#include <stdio.h>
#include <math.h>
#include <sys/random.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#define MAX 4294967295

int total_points, circle_points = 0;

int main(int argc, char const *argv[])
{
	if (argc - 1 == 0)
		return 0;
	total_points = atoi(argv[1]);
#pragma omp parallel
	{
		srand((time(NULL)) ^ omp_get_thread_num());

#pragma omp for
		for (int i = 0; i < total_points; i++)
		{

			double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
			double y = (double)rand() / RAND_MAX * 2.0 - 1.0;
			double distance = sqrt(x * x + y * y);
			if (distance <= 1)
				circle_points++;
		}
	}

	double pi = 4 * (double)circle_points / (double)total_points;
	printf("pi=%f\n", pi);
	return 0;
}
