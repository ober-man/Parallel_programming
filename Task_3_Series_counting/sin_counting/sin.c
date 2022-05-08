#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define pi 3.1415

long double fact(long double n)
{
	long double result = 1;
	for(int i = 1; i <= n; ++i)
		result *= i;

	return result;
}

long double degree(long double num, long double deg)
{
	long double result = 1;
	for(int i = 1; i <= deg; ++i)
		result *= num;
	return result;
}

// This program is counting sin(pi/6) by summing a series
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

	// sin(x) = sum_{n=1}^{inf} ((-1)^n*x^(2n+1)/(2n+1)!)
	for (int i = 0; i < num_of_steps; ++i)
	{
		number = i * size + rank;
		if (number <= N)
			sum += degree(pi/6, 2*number+1) / fact(2*number+1) * (number%2 == 1 ? -1 : 1);
	}

	// Sum all results on the process 0
	MPI_Reduce(&sum, &result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	double time_end_rank = MPI_Wtime();

	// Waiting for all processes to send their sums
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0)
		printf("sin(pi/6) = %Lg\n", result);

	printf("Process %d time = %lg mks\n", rank, (time_end_rank - time_begin_rank)*1e6);
	double time_end = MPI_Wtime();
	if(rank == 0)
		printf("Program time = %lg mks\n", (time_end - time_begin)*1e6);
	
	MPI_Finalize();
	return 0;
}	