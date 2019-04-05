#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

  void generateReduceData (int* arr, size_t n);

#ifdef __cplusplus
}
#endif



//calculate sum of array
int taskSum(int* arr, int start, int end)
{
    int task_sum = 0;
    // If there are more threads available, creating nested parallel section
#pragma omp parallel for reduction(+:task_sum)
    for(int i=start;i<end;i++)
    {
        task_sum += arr[i];
    }
    return task_sum;
}


//start time
std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

int main (int argc, char* argv[]) {

  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }
  
  if (argc < 3) {
    std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int * arr = new int [n];
  int result = 0;
  int nbthreads = atoi(argv[2]);
  int granularity = n/(2*nbthreads);
  omp_set_dynamic(0);
//set number of threads
  omp_set_num_threads(nbthreads);

//take partial sum
  int * partial_sum = new int [nbthreads];

  generateReduceData (arr, atoi(argv[1]));



#pragma omp parallel for

for(int i=0; i<n; i+=granularity){
  int j;
  int begin = i;
  static int x;
  int end = begin+granularity;
  if(end>n){
    end = n;
  }
  #pragma omp task
  {
//call function to getr sum
     x = taskSum(arr, begin, end);
     #pragma omp critical
      result+=x;
  }
 } 




//end time
      std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	std::chrono::duration<double> elpased_seconds = end-start;

//free array
  delete[] arr;



  // display result
  std::cout<<result<<std::endl;
  std::cerr<<elpased_seconds.count()<<std::endl;

  return 0;
}
