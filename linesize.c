#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include <features.h>
#include <sched.h>
#include <stdlib.h>
#include "perf.h"


#define MAX_MEM_DISTANCE 1024
#define NUM_TIMES 1000
#define NUM_ELEM 4096
volatile char *list;
unsigned long offsets[2];
int r;
//int index1=-1;
void * increase(void* in) {
    int index = *((int *) in);
    unsigned long offset = offsets[index];

    cpu_set_t cpuset1;
    CPU_ZERO(&cpuset1);
    if (index == 0){
        CPU_SET(1, &cpuset1);
    }else{
        CPU_SET(0, &cpuset1);
    }

    pthread_t threadt = pthread_self();
    pthread_setaffinity_np(threadt, sizeof(cpu_set_t), &cpuset1);

    long long int i = 0;

    for (i = 0; i < NUM_TIMES; i++ ) {
        list[offset] += 1;
		__sync_synchronize();
    }
	//printf("%d\n",list[offset]);
    return NULL; 
}



int main() {

    list = (char *)calloc(NUM_ELEM, sizeof(char));
    cpu_set_t cpusetMain;
    CPU_SET(3, &cpusetMain);
    pthread_t mainThread = pthread_self();
    pthread_setaffinity_np(mainThread, sizeof(cpu_set_t), &cpusetMain);

    cpu_set_t cpusetMainGet;
    pthread_getaffinity_np(mainThread, sizeof(cpu_set_t), &cpusetMainGet);


    pthread_t thread1, thread2;
    int idxs[2];
    idxs[0] = 0;
    idxs[1] = 1; 
	int i;	
    struct timespec start, end;

	FILE *f;
    f = fopen("linesize.txt", "a");
	
    int offset = 256;  
	int j = 0;
    double total[MAX_MEM_DISTANCE];
    for (j = 0; j <= MAX_MEM_DISTANCE; j++) {
        total[j] = 0;
    }

    for (j = 0; j < 100; j++)
	 {
        for (i = 0; i <= MAX_MEM_DISTANCE; i++) {

            offsets[0] = i+offset;
            offsets[1] = offset;
            list[offsets[0]] = 0;
            list[offsets[1]] = 0;

            clock_gettime(CLOCK_REALTIME, &start);
		
            pthread_create(&(thread1), NULL, increase, &idxs[0]);           
			pthread_create(&(thread2), NULL, increase, &idxs[1]);
	        pthread_join(thread1, NULL);
            pthread_join(thread2, NULL);

            clock_gettime(CLOCK_REALTIME, &end);

            double fixed_time = ((double)((end.tv_sec-start.tv_sec)+(end.tv_nsec-start.tv_nsec)/1e9));
            total[i] = total[i]+fixed_time;
        }
    }

    for (i = 0; i <= MAX_MEM_DISTANCE; i++) {
       // printf("Distance=%lu,\t operating time=%g\n", i * sizeof(char), total[i]/100);
		fprintf(f,"Distance=%lu,\t operating time=%g\n", i * sizeof(char), total[i]/100);
    }	

	for (i = 0; i <= 80; i++) {
        printf("Distance=%lu,\t operating time=%g\n", i * sizeof(char), total[i]/100);
	}
	fclose(f);
    return 0;
}
