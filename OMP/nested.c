#include <stdio.h>
#include <omp.h>

int main()
{
	omp_set_num_threads(2);
	omp_set_nested(3);

	#pragma omp parallel
	{
		int n1 = omp_get_num_threads();
		printf("Level 1: thread %d/%d\n", omp_get_thread_num(), n1);
		#pragma omp parallel
		{
			int n2 = omp_get_num_threads();
			printf("Level 2: thread %d/%d, first level had %d threads\n",
					omp_get_thread_num(), n2, n1);
			#pragma omp parallel
			{
				int n3 = omp_get_num_threads();
				printf("Level 3: thread %d/%d, second level had %d threads, first level had %d threads\n",
						omp_get_thread_num(), n3, n2, n1);
			}
		}
	}

	return 0;
}