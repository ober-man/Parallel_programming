#include <stdio.h>
#include <omp.h>

int n;

// threadprivate variable contains old value for master thread
// 								   and 0 for others
#pragma omp threadprivate(n)
int main()
{
	n = 1000;

	#pragma omp parallel
	{
		printf("Input threadprivate var n = %d, thread %d\n", n, omp_get_thread_num());
		n = omp_get_thread_num() + 10;
	}
	printf("\n");

	// copyin propagates threadprivate values from master-thread to others
	#pragma omp parallel copyin(n)
	{
		printf("Copyin threadprivate var n = %d, thread %d\n", n, omp_get_thread_num());
	}
	printf("\n");

	n = 100;

	#pragma omp parallel
	{
		printf("Output threadprivate var n = %d, thread %d\n", n, omp_get_thread_num());
	}
}
