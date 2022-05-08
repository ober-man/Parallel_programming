#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	if(argc < 2)
	{
		printf("Error: to less arguments\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	int N = atoi(argv[1]);
	int size, rank = 0;
	long double result = 0;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Waiting for all processes to start global time
	MPI_Barrier(MPI_COMM_WORLD);
	double time_begin = MPI_Wtime();

	long num_of_steps = N / size + 1; 
	long number = 0;
	long double sum = 0;

	double time_begin_rank = MPI_Wtime();

	// The main idea is that the series (1/n^2) from 0 to inf
	// converges to pi^2/6
	for (int i = 0; i < num_of_steps; ++i)
	{
		number = i * size + rank;
		if (number <= N && number != 0)
			sum += 1.0 / number / number;
	}

	// Sum all results on the process 0
	MPI_Reduce(&sum, &result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	double time_end_rank = MPI_Wtime();

	// Waiting for all processes to send their sums
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0)
		printf("pi = %lg\n", sqrt(6*result));

	printf("Process %d time = %lg mks\n", rank, (time_end_rank - time_begin_rank)*1e6);
	double time_end = MPI_Wtime();
	if(rank == 0)
		printf("Program time = %lg mks\n", (time_end - time_begin)*1e6);
	
	MPI_Finalize();
	return 0;
}	