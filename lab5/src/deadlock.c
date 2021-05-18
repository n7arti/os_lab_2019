#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;


pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;
void deadlock1()
{
	pthread_mutex_lock(&mut1);
	sleep(2);
	pthread_mutex_lock(&mut2);
	pthread_mutex_unlock(&mut2);
	pthread_mutex_unlock(&mut1);
}
void deadlock2()
{
	pthread_mutex_lock(&mut2);
	sleep(2);
	pthread_mutex_lock(&mut1);
	pthread_mutex_unlock(&mut1);
	pthread_mutex_unlock(&mut2);
}

int main()
{
	pthread_t thread[2];

	if (pthread_create(&thread[0], NULL, (void *)deadlock1, NULL) != 0)
	{
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&thread[1], NULL, (void *)deadlock2, NULL) != 0)
	{
		perror("pthread_create");
		exit(1);
	}
	if (pthread_join(thread[0], NULL) != 0)
	{
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(thread[1], NULL) != 0)
	{
		perror("pthread_join");
		exit(1);
	}
	return 0;
}