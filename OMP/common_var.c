#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
	int x = 0;

	#pragma omp parallel shared(x) 
	{
		int num = omp_get_thread_num();
		while(x != num)
			;
		printf("Thread number %d, x = %d\n", num, x);
		#pragma omp atomic
			++x;
	}

	return 0;
}