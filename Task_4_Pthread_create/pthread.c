#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* func(void* param)
{	
	int num = *(int*)param;
	printf("Hello, World, this is thread %d\n", num);
	return NULL;
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Too less arguments\n");
		return -1;
	}
	int N = atoi(argv[1]);

	pthread_t* ptid = (pthread_t*)calloc(N, sizeof(pthread_t));
	int* id = (int*)calloc(N ,sizeof(int));
	for(int i = 0; i < N; ++i)
	{
		id[i] = i;
		pthread_create(&ptid[i], NULL, func, &id[i]);
	}
	for(int i = 0; i < N; ++i)
		pthread_join(ptid[i], NULL);

	free(ptid);
	free(id);
	return 0;
}