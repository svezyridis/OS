#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define VALID 1
#define INVALID 0
#define GRID_SIZE 9

int numbers;
int valid;
int *args[GRID_SIZE];
short sudoku[9][9] = {{6, 2, 4, 5, 3, 9, 1, 8, 7},
		      {5, 1, 9, 7, 2, 8, 6, 3, 4},
		      {8, 3, 7, 6, 1, 4, 2, 9, 5},
		      {1, 4, 3, 8, 6, 5, 7, 2, 9},
		      {9, 5, 8, 2, 4, 7, 3, 6, 1},
		      {7, 6, 2, 3, 9, 1, 4, 5, 8},
		      {3, 7, 1, 9, 5, 6, 8, 4, 2},
		      {4, 9, 6, 1, 8, 2, 5, 7, 3},
		      {2, 8, 5, 4, 7, 3, 9, 1, 6}};
void *validate_column(void *params);
void *validate_row(void *params);
void *validate_subgrid(void *params);

int main(int argc, char const *argv[])
{
	valid = VALID;
	pthread_t column_tid[GRID_SIZE];
	pthread_t row_tid[GRID_SIZE];
	pthread_t subgrid_tid[GRID_SIZE];
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);
	for (size_t i = 0; i < GRID_SIZE; i++)
	{
		args[i] = malloc(sizeof(args[i]));
		if (args[i] == NULL)
		{
			fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
			exit(EXIT_FAILURE);
		}
		*args[i] = i;
		pthread_create(&column_tid[i], &attributes, validate_column, args[i]);
		pthread_create(&row_tid[i], &attributes, validate_row, args[i]);
		pthread_create(&subgrid_tid[i], &attributes, validate_subgrid, args[i]);
	}
	for (size_t i = 0; i < GRID_SIZE; i++)
	{
		pthread_join(column_tid[i], NULL);
		pthread_join(row_tid[i], NULL);
		pthread_join(subgrid_tid[i], NULL);
	}
	for (size_t i = 0; i < GRID_SIZE; i++)
	{
		free(args[i]);
	}
	if (valid == VALID)
	{
		printf("sudoku is valid\n");
	}
	else
	{
		printf("sudoku is not valid\n");
	}

	return 0;
}

void *validate_column(void *params)
{
	int *ptr = params;
	int column = *ptr;
	short *map = calloc(9, sizeof(short));
	for (size_t i = 0; i < 9; i++)
	{
		short num = sudoku[i][column];
		if (num > 9 || num < 1 || map[num - 1] == 1)
		{
			valid = INVALID;
			break;
		}
		map[num - 1] = 1;
	}
	free(map);
	pthread_exit(0);
}
void *validate_row(void *params)
{
	int *ptr = params;
	int row = *ptr;
	;
	short *map = calloc(9, sizeof(short));
	for (size_t i = 0; i < 9; i++)
	{
		short num = sudoku[row][i];
		if (num > 9 || num < 1 || map[num - 1] == 1)
		{
			valid = INVALID;
			break;
		}
		map[num - 1] = 1;
	}
	free(map);
	pthread_exit(0);
}
void *validate_subgrid(void *params)
{
	int *ptr = params;
	int grid = *ptr;
	short *map = calloc(9, sizeof(short));
	for (int i = 0; i < 9; i++)
	{
		short num = sudoku[(grid % 3) * 3 + i % 3][(grid / 3) * 3 + i / 3];
		if (num > 9 || num < 1 || map[num - 1] == 1)
		{
			valid = INVALID;
			break;
		}
		map[num - 1] = 1;
	}
	free(map);
	pthread_exit(0);
}
