#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Wrong entry\n");
		return EXIT_FAILURE;
	}

	double sum = 0;
	int N = atoi(argv[1]);

	/*#pragma omp parallel shared(sum)
	{
		int step_number = N / omp_get_num_threads() + 1;
		for(int i = 0; i < step_number; ++i)
		{
			int n = i*omp_get_num_threads() + omp_get_thread_num() + 1;
			if(n <= N)
			{
				#pragma omp atomic
					sum += 1.0/n;
			}
		}
	}*/

	#pragma omp parallel for shared(sum)
	for (int i = 1; i <= N; ++i)
	{
		#pragma omp atomic
			sum += 1.0/i;
	}

	printf("Sum 1 + 1/2 + ... + 1/N = %lg\n", sum);
	return 0;
}