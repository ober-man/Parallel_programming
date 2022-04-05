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
	if(rank == 0)
	{
		int* arr = (int*)calloc(size, sizeof(int));
		int* rec_arr = (int*)calloc(size, sizeof(int));
		for(int i = 0; i < size; ++i)
			arr[i] = i;
		printf("arr[%d] = %d\n", rank, rec_arr[rank]);
		MPI_Send(arr, size, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
		MPI_Recv(rec_arr, size, MPI_INT, size-1, 0, MPI_COMM_WORLD, &status);
		printf("End of sending\n");
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
		printf("arr[%d] = %d\n", rank, rec_arr[rank]);
		MPI_Send(rec_arr, size, MPI_INT, 0, 0, MPI_COMM_WORLD);
		free(rec_arr);
	}
	else
	{
		int* rec_arr = (int*)calloc(size, sizeof(int));
		MPI_Recv(rec_arr, size, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
		printf("arr[%d] = %d\n", rank, rec_arr[rank]);
		MPI_Send(rec_arr, size, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
		free(rec_arr);
	}

	MPI_Finalize();
	return 0;
}
