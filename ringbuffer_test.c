#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <pthread.h>

#include "ringbuffer.h"

#define	  FIFO_SIZE	16

void * consumer_proc(void *arg)
{
	unsigned int cnt;
	struct ringbuffer *ring_buf = (struct ringbuffer *)arg;

	char str[4] = {0};

	while(1)
	{
        printf("get data from ring buffer.\n");

        if (ringbuffer_is_empty(ring_buf)) {
            printf("buffer is empty !\n");
            sleep(1);
            continue;
        }

        ringbuffer_get(ring_buf, str, 3);

        printf("data is: %s \n", str);

        cnt++;
	}
}

void * producer_proc(void *arg)
{
	struct ringbuffer *ring_buf = (struct ringbuffer *)arg;
	char *str="1234";

	while(1)
	{
		printf("put datas to ring buffer.\n");
		

		if (ringbuffer_is_full(ring_buf)) {
			printf("buffer is full !\n");
			sleep(1);
			continue;
		}
		ringbuffer_put(ring_buf, str, 4);
		sleep(1);
	}
}


int consumer_thread(void *arg)
{
	int err;
	pthread_t tid;
	err = pthread_create(&tid, NULL, consumer_proc, arg);
	if (err != 0)
	{
		fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
				errno, strerror(errno));
		return -1;
	}
	return tid;
}
int producer_thread(void *arg)
{
	int err;
	pthread_t tid;
	err = pthread_create(&tid, NULL, producer_proc, arg);
	if (err != 0)
	{
		fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
				errno, strerror(errno));
		return -1;
	}
	return tid;
}



int main(void)
{
	struct ringbuffer *ring_buf;
	pthread_t produce_pid, consume_pid; 

	ring_buf = ringbuffer_create(FIFO_SIZE);
	if (!ring_buf) {
		perror("ringbuffer_create()");
		exit(1);
	}

	printf("multi thread test.......\n");

	produce_pid  = producer_thread((void*)ring_buf);
	consume_pid  = consumer_thread((void*)ring_buf);

	/*pthread_join(produce_pid, NULL);*/
	/*pthread_join(consume_pid, NULL);*/
    sleep(50);

	ringbuffer_destroy(ring_buf);

	return 0;
}
