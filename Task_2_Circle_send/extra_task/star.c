#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	if(argc < 2)
	{
		printf("Error: to less arguments\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	int N = atoi(argv[1]);
	int rank = 0;
	int size = 0;
	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// if there are only 1 process
	if(size == 1)
	{
		int* arr = (int*)calloc(size, sizeof(int));
		int* rec_arr = (int*)calloc(size, sizeof(int));
		for(int i = 0; i < size; ++i)
			arr[i] = N;

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

	if(rank == 0) // central process-manager
	{
		int* arr = (int*)calloc(size, sizeof(int));
		int* rec_arr = (int*)calloc(size, sizeof(int));
		for(int i = 0; i < size; ++i)
			arr[i] = N;

		printf("Process 0: arr[0] = %d\n", arr[0]);
		for(int i = 1; i < size; ++i)
		{
			// send array to i process
			MPI_Send(arr, size, MPI_INT, i, 0, MPI_COMM_WORLD);

			// and wait for a reply
			MPI_Recv(rec_arr, size, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		}

		printf("Received arr: ");
		for(int i = 0; i < size; ++i)
			printf("%d ", rec_arr[i]);
		printf("\n");
		free(arr);
		free(rec_arr);
	}
	else // process-executor
	{
		int* rec_arr = (int*)calloc(size, sizeof(int));
		MPI_Recv(rec_arr, size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		printf("Process %d: arr[%d] = %d\n", rank, rank, rec_arr[rank]);
		MPI_Send(rec_arr, size, MPI_INT, 0, 0, MPI_COMM_WORLD);
		free(rec_arr);
	}

	MPI_Finalize();
	return 0;
}
