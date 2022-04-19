#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	int N = 100; // maximal number in sum
	int size = 0; // number of processes
	int rank = 0; // rank of a process

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// if there are only 1 process
	if(size == 1)
	{
		int sum = 0;
		MPI_Status status;
		for(int i = 1; i <= N; ++i)
			sum += i;

		// send a number to itself
		MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		int sum_rec;
		MPI_Recv(&sum_rec, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		// and print it
		printf("Sum 1 + ... + N = %d\n", sum_rec);
		MPI_Finalize();
		return 0;
	}

	if(rank == 0) // process-manager
	{
		int sum = 0;
		int sum_part = 0;
		MPI_Status status;
		for(int i = 1; i < size; ++i)
		{
			MPI_Recv(&sum_part, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			sum += sum_part;
		}

		printf("Sum 1 + ... + N = %d\n", sum);
	}
	else // rank > 0  =>  process-executor
	{
		int sum_part = 0;

		// we have (size-1) processes-executors with ranks 1...(size-1)
		// each of them gets the following interval:
		int start = N / (size - 1) * (rank - 1)  + 1;
		int end = N / (size - 1) * rank;

		// sum numbers in the chosen diapason
		for(int i = start; i <= end; ++i)
			sum_part += i;

		// if N do not divides on number of processes, 
		// then the residue divides between processes too
		if(N % (size-1) != 0 &&  N % (size-1) >= rank)
			sum_part += N - (rank - 1);

		MPI_Send(&sum_part, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
