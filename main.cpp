#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <climits>
#include <pthread.h>
#include "buffer.cpp"

#define BUFF_NUM 3
#define P_NUM 2
#define C_NUM 2

using namespace std;

pthread_t prod_t[P_NUM];
pthread_t cons_t[C_NUM];

Buffer buffers[BUFF_NUM];

void produce(int item, int tid) {
        int id = rand() % BUFF_NUM;

	buffers[id].put(item);
        
        printf("Producer id %d produced item: %d to buffer: %d\n", tid, item, id);
}

int consume(int tid) {
        int id = rand() % BUFF_NUM;

        int item = buffers[id].get();

        if (item == INT_MAX) {
                //printf("Alarm received by consumer %d from queue %d. Clearing queues...\n", tid, id);
                return(INT_MAX);
        }

        printf("Consumer id %d consumed item: %d from queue: %d\n", tid, item, id);

        return(item);
}

void* prod1() {
        int i = 0, j, k;
        while (i < 5) {
                produce(i, gettid());
                ++i;
                for (j = 0; j < 10; ++j) { for (k = 0; k < 9999999; ++k) {} }
        }
}

void* prod2() {
        int i = 0, j, k;
        while (i < 3) {
                produce(i, gettid());
                ++i;
                for (j = 0; j < 15; ++j) { for (k = 0; k < 9999999; ++k) {} }
        }
        prod_alarm(gettid());
}

void* cons1() {
        int item;
        int i = 0, j, k;

        while (i < 10) {
                item = consume(gettid());
                if (item == INT_MAX) {
                        // send signal to every consumer thread
                        for (j = 0; j < C_NUM; ++j) {
                                pthread_kill(cons_t[j], SIGUSR1);
                        }
                        exit(0);
                }
                ++i;
                for (j = 0; j < 15; ++j) { for (k = 0; k < 9999999; ++k) {} }
        }
}

void* cons2() {
        int item;
        int i = 0, j, k;

        while (i < 10) {
                item = consume(gettid());
                if (item == INT_MAX) {
                        // send signal to every consumer thread
                        for (j = 0; j < C_NUM; ++j) {
                                pthread_kill(cons_t[j], SIGUSR1);
                        }
                        exit(0);
                }
                ++i;
                for (j = 0; j < 20; ++j) { for (k = 0; k < 9999999; ++k) {} }
        }
}

int main(int argc, char** argv) {
	srand(time(NULL));
	singal(SIGUSR1, sig_hander);

	pthread_create(&prod_t[0], NULL, prod1, NULL);
        pthread_create(&prod_t[1], NULL, prod2, NULL);
        pthread_create(&cons_t[0], NULL, cons1, NULL);
        pthread_create(&cons_t[1], NULL, cons2, NULL);

        pthread_join(prod_t[0], NULL);
        pthread_join(prod_t[1], NULL);
        pthread_join(cons_t[0], NULL);
        pthread_join(cons_t[1], NULL);

	return 0;
}
