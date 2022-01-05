#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <climits>
#include <pthread.h>
#include <signal.h>
#include "buffer.cpp"

#define BUFF_NUM 3
#define P_NUM 2
#define C_NUM 2

using namespace std;

pthread_t prod_t[P_NUM];
pthread_t cons_t[C_NUM];

Buffer buffers[BUFF_NUM];

bool isAlarm;

void alarm(int tid) {
	printf("Producent %d produced alarm\n", tid);
	isAlarm = true;
}

void produce(int item, int tid) {
        int id = rand() % BUFF_NUM;

	buffers[id].put(item);
        
        printf("Producer id %d produced item: %d to buffer: %d\n", tid, item, id);
}

int consume(int tid) {
        int id = rand() % BUFF_NUM;

        int item = buffers[id].get();

        printf("Consumer id %d consumed item: %d from buffer: %d\n", tid, item, id);

        return(item);
}

void* prod1(void* null) {
        int i = 0, j, k;
        while (i < 10) {
		if (isAlarm) pthread_exit(NULL);

                produce(i, gettid());
                ++i;
                for (j = 0; j < 5; ++j) { for (k = 0; k < 9999999; ++k) {} }
        }
}

void* prod2(void* null) {
        int i = 0, j, k;
        while (i < 2) {
		if (isAlarm) pthread_exit(NULL);

                produce(i, gettid());
                ++i;
                for (j = 0; j < 15; ++j) { for (k = 0; k < 9999999; ++k) {} }
        }
	alarm(gettid());
}

void* cons1(void* null) {
        int item;
        int i = 0, j, k;

        while (i < 4) {
                item = consume(gettid());
		if (isAlarm) {
			printf("Consumer %d received alarm during conusming item %d. Quitting...\n", gettid(), item);
			pthread_exit(NULL);
		}
                ++i;
                for (j = 0; j < 40; ++j) { for (k = 0; k < 9999999; ++k) {} }
        }
}

void* cons2(void* null) {
        int item;
        int i = 0, j, k;

        while (i < 4) {
                item = consume(gettid());
		if (isAlarm) {
			printf("Consumer %d received alarm during conusming item %d. Quitting...\n", gettid(), item);
			pthread_exit(NULL);
		}
                ++i;
                for (j = 0; j < 25; ++j) { for (k = 0; k < 9999999; ++k) {} }
        }
}

int main(int argc, char** argv) {
	srand(time(NULL));
	isAlarm = false;

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
