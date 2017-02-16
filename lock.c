#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "math.h"
#include <assert.h>
#include <semaphore.h>

#define MAX_THREADS  16
#define INTER        10
#define CYCLES       1024*1024
int t_num = 1;
int mode = 0;
int memlocation = 0;

sem_t Sem;

struct timespec start[16];
struct timespec end[16];
void atomicIncr(int * loc) {
    asm("lock incl (%0)" : : "r" (loc) : "0");
}

void SemInc(int * loc) {
    sem_wait(&Sem);
    (*loc) += 1;
    sem_post(&Sem);
}

void * increase(void* input) {

    //looks up value of t_num
    int idx = *((int *) input);
    int num_adds = CYCLES/t_num;
    int i = 0;

    clock_gettime(CLOCK_REALTIME, &(start[idx]));
    if (!mode) 
	{
        for (i = 0; i < num_adds; i++) 
		{
            atomicIncr(&memlocation);
        }
    }
    else 
	{
        for (i = 0; i < num_adds; i++) 
		{
            SemInc(&memlocation);
        }
    }

    clock_gettime(CLOCK_REALTIME, &(end[idx]));

    return NULL; 
}


double func()
{
    memlocation = 0;
    int i = 0;
    pthread_t threads[t_num];
    int inputs[t_num];

    sem_init(&Sem, 0, 1);

    inputs[0] = 0;
    increase(&inputs[0]);
    for(i= 1; i < t_num; i++)
	{
        inputs[i] = i;
        pthread_create(&(threads[i]), NULL, increase, &(inputs[i]));
	
    }
    for (i = 1; i < t_num; i++) 
	{
        pthread_join(threads[i], NULL);
    }
	double time_max=end[0].tv_sec+end[0].tv_nsec/1e9;
	double time_min=start[0].tv_sec+start[0].tv_nsec/1e9;
	double temp1,temp2;

    for(i = 1; i <t_num ; i++)  //
	{
		temp1=end[i].tv_sec+end[i].tv_nsec/1e9;
		temp2=start[i].tv_sec+start[i].tv_nsec/1e9;
		if (time_max<temp1)
			time_max=temp1;
		if (time_min>temp2)        
			time_min=temp2;

		//printf("max=%g,min=%g\n",time_max,time_min);
		//printf("end_S=%g,end_ns=%g\n",end[i].tv_sec,end[i].tv_nsec);
		//printf("start_s=%g,start_ns=%g\n",start[i].tv_sec,start[i].tv_nsec);
	}   
	double fixed_time=(double) ((time_max-time_min));
    __sync_synchronize();
    return fixed_time;

} 


int main() {
    struct timespec start_a, end_a;
    struct timespec start_s, end_s;
	int i,j,k;
	FILE *f;
    f = fopen("lock.txt", "a");
    for (j = 1; j <=MAX_THREADS; j++) 
	{
        t_num = j;     
        clock_gettime(CLOCK_REALTIME, &start_a);
        double running_sum = 0;
        mode = 0;
        for (k = 0; k < INTER; k++) 
		{
            running_sum += func();
        }
        clock_gettime(CLOCK_REALTIME, &end_a);
        printf("atomic thread=%d, time=%g\n", j, running_sum / (double) INTER);
		fprintf(f, "atomic thread=%d, time=%g\n", j, running_sum / (double) INTER);
    }
    for (j = 1; j <= MAX_THREADS; j++) 
	{
        t_num = j;  
        clock_gettime(CLOCK_REALTIME, &start_s);
        double running_sum = 0;
        mode = 1;
		int k;
        for ( k= 0; k < INTER; k++) 
		{
            running_sum += func();
        }
        clock_gettime(CLOCK_REALTIME, &end_s);
        printf("semaphore thread=%d, time=%g\n", j, running_sum / (double) INTER );
		fprintf(f, "semaphore thread=%d, time=%g\n", j, running_sum / (double) INTER);
    }
	fclose(f);
    return 0;
}
