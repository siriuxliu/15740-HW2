#include <vector>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <chrono>
extern "C" {
#include "func_time.h"
}

using namespace std;

void *work(void *) {
  auto start = std::chrono::system_clock::now();
  long sum=0;
  for (long i=0; i < 1000000000; i++){
    sum=i+i;
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double, std::milli> duration_ms = end - start;
  std::cout << "thread elapsed wall clock: "<<duration_ms.count() << "ms" << std::endl;   
  pthread_exit(NULL);
}

void thread_test(int num_threads) {
  std::vector<pthread_t> threads(num_threads);
  pthread_attr_t attr;
  void *status;
  int rc;
  int i;

  auto start = std::chrono::system_clock::now();

  // Initialize and set thread joinable
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for( i=0; i < num_threads; i++ ){
    //    cout <<"main() : creating thread, " << i << endl;
    rc = pthread_create(&threads[i], NULL, work, NULL);
		
    if (rc){
      cout << "Error:unable to create thread," << rc << endl;
      exit(-1);
    }
  }

  // free attribute and wait for the other threads
  pthread_attr_destroy(&attr);	
  for( i=0; i < num_threads; i++ ){
    rc = pthread_join(threads[i], &status);
    if (rc){
      cout << "Error:unable to join," << rc << endl;
      exit(-1);
    }
  }

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double, std::milli> duration_ms = end - start;
  std::cout << "process elapsed wall clock: "<<duration_ms.count() << "ms" << std::endl;   

    //    cout << "Main: completed thread id :" << i ;
    // cout << "  exiting with status :" << status << endl;
//  pthread_exit(NULL);
}

int main(){
  for (int i=1; i < 20; i++){
    cout << "======= " << i << " threads ======" << endl;
    thread_test(i);
    // long double secs;
    // secs = func_time(thread_test, i, 0.02);
    // cout << "cpu time: " << secs*1000 << " ms" << endl;
    // secs = func_time_hw(thread_test, i, 0.02);
    // cout << "wall clock time: " << secs*1000 << " ms" << endl;
  }
}
