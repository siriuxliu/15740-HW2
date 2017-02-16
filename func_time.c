#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include "perf.h"
#include "func_time.h"

typedef void (*timer_init_funct)(void);
typedef long double (*timer_elapsed_funct)(void);

static long double delta = -1.0;
static long double delta_hw = -1.0;
void get_delta()
{
	long double t1,t2;   	
	init_etime();
	t1=t2=get_etime();
	while (t1==t2)
		t2=get_etime();
	delta=t2-t1;
    printf("Time_interval=%LE\n",delta);	
}

long double func_time_generic(test_funct P, long double E) 
{
	int cnt = 1;
  	
  	double ta = 0.0;
  	if (delta < 0) 
	{
    	get_delta();
  	}
//  	tmin = delta / E;
	
	double tmin=2.0*delta/E + delta;  	
	while (ta < tmin) {
    	int c = cnt;
			init_etime();
    	double st = get_etime();
    	while (c-- > 0) {
      		P();
    	}
    	ta = get_etime() - st;
    	if (ta < tmin)
      		cnt=cnt*2;
  	}
  	return ta / cnt;
}



long double func_time(test_funct P, long double E) {
    long double result = func_time_generic(P, E);
    return result;
}


