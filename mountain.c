#include "func_time.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

#define CMAX 1024L*1024L*512L
#define SMAX 45
#define JUMP 10

char* memory;

unsigned long long int n, s;
int r=16;
void better()
{
	double result = 0.0;
    volatile double sink;
    unsigned long long int i;
    unsigned long long int j;
    unsigned long long range = 128;
    unsigned long long coeff1 = (n * s) / range;

    for (i=0; i<n/range; i++){
        for(j = 0; j < range; j++) {
			
            result = result + (double)memory[((i*s) + (j * coeff1)+r)%(1024L*1024L*512L)];
        }
    }
    sink = result;
}


void simple(){
    double result = 0.0;
    volatile double sink;
    unsigned long long int i;
    for (i=0; i<n; i++){
        result += (double)memory[(i*s)%(CMAX)];
    }
    sink = result;
}


int main(int argc, char** argv){
    
    unsigned long long int i;

    unsigned long long int tot;
    
    memory = (char*)calloc(CMAX, 1);
    tot = SMAX;
    tot = tot * CMAX;

	FILE *f;
    f = fopen("mountain.txt", "a");
   	if (argc==1)
	{
		printf("No argument\n");
		printf("-h for help\n");
		return 0;
	}
	int pb=strcmp(argv[1],"-b");
	int ps=strcmp(argv[1],"-s");
	int ph=strcmp(argv[1],"-h");
    
    if(ps==0&&pb!=0&&ph!=0)
		printf("simple\n");
	else if (pb==0&&ps!=0&&ph!=0)
		printf("better\n");
    else if (ph==0&&ps!=0&&pb!=0)
	{		
		printf("Help:\t\n\t-s\tsimple\n\t-b\t better\n");
		return 0;	
	}	
   	else
	{
		printf("Wrong argument\n");
		printf("Help:\t\n\t-s\tsimple\n\t-b\tbetter\n");
		return 0;
	}

    long double secs;
    unsigned long long int j;

	if(ps==0&&pb!=0)
	{	
		for(j = 256L; j<pow(2,28); j=j*2)
		{
         	for (i=1; i<=SMAX; i=i+2)
			{
             	s = i;
            	n = j;
				secs = func_time(simple, 0.02);
				printf("%Ld\t %f\t %Le\n", s, log2(j), n/(secs*1e6));
				fprintf(f, "%Ld\t %f\t %Le\n", s, log2(j), n/(secs*1e6)); 	
	        }
		
		}
	}			
	else if (pb==0&&ps!=0)
	{
		for(j = 512L; j<pow(2,28); j=j*2)
		{
	    	for (i=5; i<=SMAX; i=i+2)
			{
           		s = i;
             	n = j;
				r=random()%32;
	         	secs = func_time(better, 0.02); 
             	printf("%Ld\t %f\t %Le\n", s-4, log2(j), n/(secs*1e6));//,secs);
				fprintf(f, "%Ld\t %f\t %Le\n", s, log2(j), n/(secs*1e6));
         	} 
		}
	}


	fclose(f);
	return 0;
}
