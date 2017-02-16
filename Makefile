CC = gcc
AS = as
CFLAGS = -O
LFLAGS =  -lrt -lm
CXXFLAGS = -O0 --std=c++11
CXXLDFLAGS = -lrt -lm -lpthread -fopenmp
CXX = g++
		 
all: mountain linesize lock cores smt mmt

func_time.o: func_time.c func_time.h
	$(CC) $(CFLAGS) -c func_time.c

perf.o: perf.c perf.h
	$(CC) $(CFLAGS) -c perf.c

mountain: mountain.c func_time.o perf.o
	$(CC) $(CFLAGS) mountain.c perf.o func_time.o -o mountain $(LFLAGS) 

linesize: linesize.c linesize.o 
	$(CC) -g $(CFLAGS) linesize.c perf.o -o linesize -lpthread $(LFLAGS)

lock: lock.c perf.o func_time.o
	$(CC) -g lock.c perf.o func_time.o -o lock -lpthread  $(LFLAGS)

cores: cores.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(CXXLDFLAGS)

smt: smt.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(CXXLDFLAGS)

mmt: mmt.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(CXXLDFLAGS)

#mountain: mountain.c perf.o
#	$(CC) $(CFLAGS) mountain.c perf.o -o mountain $(LFLAGS) 

clean:
	rm -f *.o *.png *.txt mountain perf func_time linesize lock cores smt mmt *~

#core: core.c perf.o func_time.o
#	$(CC) -g $(CFLAGS) core.c perf.o func_time.o -o core -lpthread  $(LFLAGS)

#smt: smt.c perf.o func_time.o
#	$(CC) -g smt.c perf.o func_time.o -o smt -lpthread  $(LFLAGS)

#matrix: matrix.c perf.o func_time.o
#	$(CC) -g matrix.c perf.o func_time.o -o matrix -lpthread  $(LFLAGS)


#clock_test: pthread_clock_test.c
#	$(CC) -g $(CFLAGS) pthread_clock_test.c -o clock_test -lpthread -lrt


