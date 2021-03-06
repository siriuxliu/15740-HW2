#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;

#define BLOCK 16 // 1-dimension block size
#define THREADS 4 // number threads

void bmm(int* a, int* b, int* c, uint msz){
  int ib, jb, kb;
  for (ib = 0; ib < msz; ib+=BLOCK) {
    for (jb = 0; jb < msz; jb+=BLOCK) {
      for (kb = 0; kb < msz; kb+=BLOCK) {
        #pragma omp parallel for num_threads(THREADS) collapse(2)
        for (int i=ib; i<(ib+BLOCK); i++){
          for (int j=jb; j<(jb+BLOCK); j++){
            for (int k=kb; k<(kb+BLOCK); k++){
              c[i*msz+j] += a[i*msz+k] * b[k*msz+j];
            }
          }
        }
      }
    }
  }
}

void initialize(int* a, int* b, uint msz){
  for (uint i=0; i<msz*msz; i++){
    a[i] = i;
    b[i] = i+1;
  }
  //  cout<< "finished initialization " << endl;
}

int main (int argc, char *argv[]){
  //uint msz = atoi(argv[1]);
  // cout<< "size of matrix " << msz << "x" << msz << endl;
  uint msz = BLOCK;
  // warm up
  // initialize(a,b,msz);
  // bmm(a,b,c,msz);
  for (int idx=1; idx<17; idx++){
     msz = idx * BLOCK;
    // msz = 3200;
     int * a = new int[msz*msz];
     int * b = new int[msz*msz];
     int * c = new int[msz*msz];
     initialize(a,b,msz);
     auto start = std::chrono::system_clock::now();
     bmm(a,b,c,msz);        
     auto end = std::chrono::system_clock::now();
     std::chrono::duration<double, std::milli> duration_ms = end - start;
     //     std::cout << "size of matrix: " << std::setw(3) << msz << "x" << std::setw(3) << msz;
     std::cout << "size of matrix: ";
     std::cout << std::setw(3) << msz << "x" ;
     std::cout << std::setw(3) << msz << ",";
     std::cout << " execution time (ms): ";
     std::cout << std::setw(10) << duration_ms.count()<<",";
     std::cout << " bandwidth (MB/s): ";
     std::cout << std::setw(10) << msz*msz*4/duration_ms.count() << std::endl;
  };
  // cout<< "final elements:" << endl;
  // for (int i=0; i< msz*msz; i++){
  //   cout<< c[i] << endl;
  // }
}
