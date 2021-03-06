#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
	int rank = 0;
	int size = 0;
	MPI_Status status;
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// if there are only 1 process
	if(size == 1)
	{
		int* arr = (int*)calloc(size, sizeof(int));
		int* rec_arr = (int*)calloc(size, sizeof(int));
		for(int i = 0; i < size; ++i)
			arr[i] = i;

		// send an array to itself
		MPI_Send(arr, size, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Recv(rec_arr, size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		// and print it
		printf("Received arr: ");
		for(int i = 0; i < size; ++i)
			printf("%d ", rec_arr[i]);
		printf("\n");
		free(arr);
		free(rec_arr);
		MPI_Finalize();
		return 0;
	}

	if(rank == 0)
	{
		int* arr = (int*)calloc(size, sizeof(int));
		int* rec_arr = (int*)calloc(size, sizeof(int));
		for(int i = 0; i < size; ++i)
			arr[i] = i;
		printf("Process 0: arr[0] = %d\n", arr[0]);

		MPI_Send(arr, size, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
		MPI_Recv(rec_arr, size, MPI_INT, size-1, 0, MPI_COMM_WORLD, &status);
		printf("Received arr: ");
		for(int i = 0; i < size; ++i)
			printf("%d ", rec_arr[i]);
		printf("\n");
		free(arr);
		free(rec_arr);
	}
	else if(rank == size-1)
	{
		int* rec_arr = (int*)calloc(size, sizeof(int));
		MPI_Recv(rec_arr, size, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
		printf("Process %d: arr[%d] = %d\n", rank, rank, rec_arr[rank]);
		MPI_Send(rec_arr, size, MPI_INT, 0, 0, MPI_COMM_WORLD);
		free(rec_arr);
	}
	else
	{
		int* rec_arr = (int*)calloc(size, sizeof(int));
		MPI_Recv(rec_arr, size, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
		printf("Process %d: arr[%d] = %d\n", rank, rank, rec_arr[rank]);
		MPI_Send(rec_arr, size, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
		free(rec_arr);
	}

	MPI_Finalize();
	return 0;
}
