#include <stdio.h>
#include <omp.h>

int task(int n)
{
	return n*n;
}

#define SIZE 65

int main()
{
	omp_set_num_threads(4);
	double tstart;

	printf("============DEFAULT CYCLE============\n");
	tstart = omp_get_wtime();
	#pragma omp parallel for
	for (int i = 1; i <= SIZE; ++i)
	{
		int res = task(i);
		printf("Thread %d/%d, task %d completed: res = %d\n", 
				omp_get_thread_num(), omp_get_num_threads(), i, res);
	}
	printf("____TIME____ = %lg\n", omp_get_wtime() - tstart);
	printf("\n\n");


	printf("============STATIC(1) BALANCED CYCLE============\n");
	tstart = omp_get_wtime();
	#pragma omp parallel for schedule(static, 1)
	for (int i = 1; i <= SIZE; ++i)
	{
		int res = task(i);
		printf("Thread %d/%d, task %d completed: res = %d\n", 
				omp_get_thread_num(), omp_get_num_threads(), i, res);
	}
	printf("____TIME____ = %lg\n", omp_get_wtime() - tstart);
	printf("\n\n");


	printf("============STATIC(2) BALANCED CYCLE============\n");
	tstart = omp_get_wtime();
	#pragma omp parallel for schedule(static, 2)
	for (int i = 1; i <= SIZE; ++i)
	{
		int res = task(i);
		printf("Thread %d/%d, task %d completed: res = %d\n", 
				omp_get_thread_num(), omp_get_num_threads(), i, res);
	}
	printf("____TIME____ = %lg\n", omp_get_wtime() - tstart);
	printf("\n\n");


	printf("============DYNAMIC(1) BALANCED CYCLE============\n");
	tstart = omp_get_wtime();
	#pragma omp parallel for schedule(dynamic, 1)
	for (int i = 1; i <= SIZE; ++i)
	{
		int res = task(i);
		printf("Thread %d/%d, task %d completed: res = %d\n", 
				omp_get_thread_num(), omp_get_num_threads(), i, res);
	}
	printf("____TIME____ = %lg\n", omp_get_wtime() - tstart);
	printf("\n\n");


	printf("============DYNAMIC(2) BALANCED CYCLE============\n");
	tstart = omp_get_wtime();
	#pragma omp parallel for schedule(dynamic, 2)
	for (int i = 1; i <= SIZE; ++i)
	{
		int res = task(i);
		printf("Thread %d/%d, task %d completed: res = %d\n", 
				omp_get_thread_num(), omp_get_num_threads(), i, res);
	}
	printf("____TIME____ = %lg\n", omp_get_wtime() - tstart);
	printf("\n\n");


	printf("============GUIDED(1) BALANCED CYCLE============\n");
	tstart = omp_get_wtime();
	#pragma omp parallel for schedule(guided, 1)
	for (int i = 1; i <= SIZE; ++i)
	{
		int res = task(i);
		printf("Thread %d/%d, task %d completed: res = %d\n", 
				omp_get_thread_num(), omp_get_num_threads(), i, res);
	}
	printf("____TIME____ = %lg\n", omp_get_wtime() - tstart);
	printf("\n\n");


	printf("============GUIDED(2) BALANCED CYCLE============\n");
	tstart = omp_get_wtime();
	#pragma omp parallel for schedule(guided, 2)
	for (int i = 1; i <= SIZE; ++i)
	{
		int res = task(i);
		printf("Thread %d/%d, task %d completed: res = %d\n", 
				omp_get_thread_num(), omp_get_num_threads(), i, res);
	}
	printf("____TIME____ = %lg\n", omp_get_wtime() - tstart);
	printf("\n\n");

	return 0;
}