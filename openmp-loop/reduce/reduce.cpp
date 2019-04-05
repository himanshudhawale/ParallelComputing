#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif

    void generateReduceData (int* arr, size_t n);

#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {

    if (argc < 5) {
        std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <chunkSize>"<<std::endl;
        return -1;
    }
    omp_set_num_threads(atoi(argv[2]));
    std::string staticStr ("static");
    std::string dynamicStr ("dynamic");
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
//chunksize
    int chunkSize = atoi(argv[4]);

    int * arr = new int [atoi(argv[1])];

    generateReduceData (arr, atoi(argv[1]));

    std::chrono::high_resolution_clock::time_point start;

    //write code here
    int result = arr[0];
    if(staticStr.compare(argv[3])==0){
        omp_set_schedule(omp_sched_static, chunkSize);
    }
    else if(dynamicStr.compare(argv[3])==0){
        omp_set_schedule(omp_sched_dynamic, chunkSize);
    }
    else 
    {
        omp_set_schedule(omp_sched_guided, chunkSize);
    }
//start time
    start = std::chrono::high_resolution_clock::now();

//use reduction formula because we see a reduction logic here
#pragma omp parallel reduction(+:result)
    {


#pragma omp for schedule(runtime)
        for(int i=1; i < atoi(argv[1]); i++ )
            result += arr[i];
    }
//end time
    std::chrono::high_resolution_clock::time_point end  = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;

//count and print on error stream
    std::cerr<<elapsed_seconds.count()<<std::endl;

    delete[] arr;

    std::cout<<result<<std::endl;
    return 0;
}
