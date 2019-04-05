#include <iostream>
#include <chrono>
#include <ratio>
#include <ctime>

#include <omp.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


#ifdef __cplusplus
extern "C" {
#endif

    void generatePrefixSumData (int* arr, size_t n);
    void checkPrefixSumResult (int* arr, size_t n);


#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
        return -1;
    }
    //forces openmp to create the threads beforehand
    omp_set_dynamic(0);
    omp_set_num_threads(atoi(argv[2]));
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

    int i, n = atoi(argv[1]);
    int * arr = new int [n];
    int * pr = new int [n+1];
    pr[0]=0;
    int nbthreads = atoi(argv[2]);

    int *temp ;


#pragma omp_set_schedule(omp_sched_static, chunkSize);

    generatePrefixSumData (arr, atoi(argv[1]));

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int sum = 0;

#pragma omp single
        {
            nbthreads = omp_get_num_threads();
            temp = new int[nbthreads];
        }
#pragma omp for schedule(static)
        for ( i = 0; i<n; i++ )
        {
            sum += arr[i];
            pr[i+1] = sum;
        }
#pragma omp critical
        temp[id] = sum;
#pragma omp barrier
        int x = 0;
        for (i=0; i<id; i++)
        {
            x += temp[i]; 
        }
#pragma omp for schedule(static)
        for (i=0;i< n;i++ )
        {
            pr[i+1] += x;
        }
    }
    std::chrono::high_resolution_clock::time_point end  = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;

    checkPrefixSumResult(pr, atoi(argv[1]));

    std::cerr<<std::fixed<<elapsed_seconds.count()<<std::endl;

    delete[] arr;
    delete[] pr;
    delete[] temp;

    return 0;
}
