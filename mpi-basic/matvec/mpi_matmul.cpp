#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <time.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

int main (int argc, char* argv[]) {

  if (argc < 3) {
    std::cerr<<"usage: mpirun "<<argv[0]<<" <n> <k>"<<std::endl;
    return -1;
  }
  
  int p, P;
  
  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &p);
  MPI_Comm_size(MPI_COMM_WORLD, &P);

  unsigned long n = atoi(argv[1]);
  unsigned int k = atoi(argv[2]);
  
  if (P != 1 && P != 4 && P != 9 && P != 16 && P != 25) {
    std::cerr<<"Use 1, 4, 9, 16, or 25 for P"<<std::endl;
    return -1;
  }
  
  
  unsigned long int rc = sqrt(P);
  //Use a multiple of rc for n
  n = (n/rc) * rc;
  unsigned long int b_row = p/rc;
  unsigned long int b_col = p%rc;
  
  //initialize the matrix and vector
  unsigned long size = n/rc;
  
  float** block = new float*[size];
  for (int i=0; i<size; i++) {
    block[i] = new float[size];
    if (block[i] == NULL) {
      std::cerr<<"Could not allocate the required memory."<<std::endl;
      return -1;
    }
  }

  float* x = new float[n];
 
  srand(time(NULL)); 

  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++) {
      block[i][j] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    }
  }
  if (p==0) {
    for (int i=0; i<n; i++) {
      x[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    }
  }
  MPI_Bcast(x, n, MPI_FLOAT, 0, MPI_COMM_WORLD);
  
  //setup communicators
  MPI_Comm b_row_comm, b_col_comm;
  MPI_Comm_split(MPI_COMM_WORLD, b_row, p, &b_row_comm);
  MPI_Comm_split(MPI_COMM_WORLD, b_col, p, &b_col_comm);
  
  //declare vars
  float* temp = new float[size];
  float* x_part = new float[size];
  int i,j;
  
  auto start = std::chrono::system_clock::now();
  
  //multiply matrix k times
  for (int K=0; K<k; K++) {
    //zero temp buffer
    for (i=0; i<size; i++)
      temp[i] = 0;
   
    for (i=0; i<size; i++) {
      for (j=0; j<size; j++) {
        temp[j] += x[(b_col*size)+j] * block[i][j];
      }
    }

    MPI_Reduce(temp, x_part, size, MPI_FLOAT, MPI_SUM, 0, b_row_comm);
   
    MPI_Bcast(x_part, size, MPI_FLOAT, 0, b_row_comm);

    MPI_Allgather(x_part, size, MPI_FLOAT, x, size, MPI_FLOAT, b_col_comm);
    
  }
  
  MPI_Barrier(MPI_COMM_WORLD);
  if (p == 0) {
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cerr<<diff.count()<<std::endl;
  }
  
  MPI_Finalize();
  return 0;
}
