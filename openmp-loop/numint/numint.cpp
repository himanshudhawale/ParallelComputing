#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<chrono>


using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif



int main (int argc, char* argv[]) {

    if (argc < 9) {
        std::cerr<<"Usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>   <scheduling> <granularity>"<<std::endl;
    	return -1;
     }

  int functionid = stoi(argv[1]);
  float a = stof(argv[2]);
  float b = stof(argv[3]);
  int n = stoi(argv[4]);
  int intensity = stoi(argv[5]);
  float mid = ((b - a) / n );
  int nbthreads = stoi(argv[6]);
  string scheduling = argv[7];
  int  granularity = stoi(argv[8]);
  float result = 0.0;

  omp_set_num_threads(nbthreads);


  if("static" == scheduling){
    omp_set_schedule(omp_sched_static, granularity);
  } else if("dynamic" == scheduling){
    omp_set_schedule(omp_sched_dynamic, granularity);
  }else if("guided" == scheduling){
    omp_set_schedule(omp_sched_guided, granularity);
  }


  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

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
//use reduction on every logic inside cases of switch 
 #pragma omp for reduction (+ : result) schedule (runtime)

   for (int i=0; i<n; i++){
     float x = (a + (i + 0.5) * mid);

     switch(functionid){
       case 1:
         result +=(f1(x, intensity));
         break;

       case 2:
         result +=(f2(x, intensity));
         break;

       case 3:
         result +=(f3(x, intensity));
         break;

       case 4:
         result +=(f4(x, intensity));
         break;
     }
   } 

  }
  //end time
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  

  std::cout<< (result*mid) <<std::endl;
  
//print execution time on error stream
  std::cerr<<elapsed_seconds.count()<<std::endl;

  return 0;
}
