#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif

using namespace std;

void merge(int* arr, int l, int m, int r)
{
    int i, j, k;
//number of elements in left array
    int nleft = m - l + 1;

//number of elements in right array 
    int nright =  r - m;

//temp left array
    int* temp_left = new int[nleft];

//temp right array 

    int* temp_right = new int[nright];

//copying elements to temp left array
    for (i = 0; i < nleft; i++)
        temp_left[i] = arr[l + i];

//copying elements to right temp array
    for (j = 0; j < nright; j++)
        temp_right[j] = arr[m + 1+ j];

 //left array  
    i = 0; 
//right array
    j = 0; 

//merged
    k = l;

 
    while (i < nleft && j < nright)
    {
        if (temp_left[i] <= temp_right[j])
        {
            arr[k] = temp_left[i];
            i++;
        }
        else
        {
            arr[k] = temp_right[j];
            j++;
        }
        k++;
    }

    //copying elements if remaining  to left temp array
    while (i < nleft)
    {
        arr[k] = temp_left[i];
        i++;
        k++;
    }

   //copying elements if remaining to right temp array
    while (j < nright)
    {
        arr[k] = temp_right[j];
        j++;
        k++;
    }

//free 
    delete[] temp_left;
    delete[] temp_right;
}

void mergeSort(int* arr, int l, int r, int nothreads)
{
   //Set the number of threads
    omp_set_num_threads(nothreads);

    int n=r;

    for(int k = 1; k < n+1;k *= 2)
    {
        #pragma omp parallel for schedule(static, 1)
        for(int i=0;i < n+1;i += (2*k))
        {
            int left = i;
            int mid = i + (k-1);
            int right = i + ((2*k)-1);
            if(mid >= n)
            {
                mid = (i+n-1)/2;
                right = n-1;
            }
            else if(right >= n)
            {
                right = n-1;
            }

            merge(arr,left,mid,right);
        }
    }
}



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

  if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nothreads = atoi(argv[2]);

  // get arr data
  int * arr = new int [n];

  generateMergeSortData (arr, n);



  // start timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  mergeSort(arr, 0, n-1, nothreads);


  // end time
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapased_seconds = end-start;

  //execution time to err  stream
  cerr<<elapased_seconds.count()<<std::endl;

  checkMergeSortResult (arr, n);


  delete[] arr;

  return 0;
}
