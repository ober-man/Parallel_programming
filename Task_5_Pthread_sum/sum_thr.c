#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct argument
{
	int num;
	int id;
	int threads;
};

pthread_mutex_t mutex;
int res = 0;

void* count(void* args)
{
	struct timespec time_begin_thread, time_end_thread;
	clock_gettime(CLOCK_REALTIME, &time_begin_thread);

	struct argument arg = *(struct argument*)args;
	int cnt = 0;
	int number = arg.num / arg.threads + 1;
	for(int i = 0; i < number; ++i)
	{
		int term = i * arg.threads + arg.id;
		if(term <= arg.num)
			cnt += term;
	}

	// critical section
	pthread_mutex_lock(&mutex);
	res += cnt;
	pthread_mutex_unlock(&mutex);

	clock_gettime(CLOCK_REALTIME, &time_end_thread);
	printf("Thread %d time = %lg mks\n", arg.id, (time_end_thread.tv_sec - time_begin_thread.tv_sec + 
													(time_end_thread.tv_nsec - time_begin_thread.tv_nsec)/1e9));

	return NULL;
}

// arguments: 
// argv[1] = N - number that we are counting to
// argv[2] = size - number of threads
int main(int argc, char* argv[])
{
	struct timespec time_begin, time_end;
	clock_gettime(CLOCK_REALTIME, &time_begin);

	if(argc < 3)
	{
		printf("Too less arguments\n");
		return -1;
	}
	int N = atoi(argv[1]);
	int size = atoi(argv[2]);
	pthread_t* ptid = (pthread_t*)calloc(size, sizeof(pthread_t));
	struct argument* args = (struct argument*)calloc(size, sizeof(struct argument));
	pthread_mutex_init(&mutex, NULL);

	for(int i = 0; i < size; ++i)
	{
		args[i].num = N;
		args[i].id = i;
		args[i].threads = size;
		pthread_create(&ptid[i], NULL, count, (void*)&args[i]);
	}
	for(int i = 0; i < size; ++i)
		pthread_join(ptid[i], NULL);
	clock_gettime(CLOCK_REALTIME, &time_end);

	printf("Sum 1 + ... + N = %d\n", res);
	printf("Program time = %lg mks\n", (time_end.tv_sec - time_begin.tv_sec + 
										(time_end.tv_nsec - time_begin.tv_nsec)/1e9));

	free(ptid);
	pthread_mutex_destroy(&mutex);
	return 0;
}