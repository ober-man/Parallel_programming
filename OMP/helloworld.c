#include <stdio.h>
#include <omp.h>

int main()
{
	#pragma omp parallel
	{
		printf("Hello, world, it is thread number %d\n", omp_get_thread_num());
	}

	return 0;
}
