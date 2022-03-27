#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv); // makes parallel section

	int rank = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // return current rank in param

	int size = 0;
	MPI_Comm_size(MPI_COMM_WORLD, &size); // return current size in param
	printf("Size is %d, rank is %d\n", size, rank);

	MPI_Finalize(); // end of parallel section

	return 0;
}
