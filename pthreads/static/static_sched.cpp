#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>

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

struct my_struct
{

  float result, sum;
  int ub, lb,functionid,intensity;
  string sync;
  float a, b, mid;

};


pthread_mutex_t mut;
float total_sum = 0;

void* numerical_integration(void* p)
{

  my_struct *struct_info = (my_struct*)p;

if(0 == struct_info->sync.compare("iteration"))
{

  switch(struct_info->functionid)
  {
    case 1:
      for (int i=struct_info->lb; i<=struct_info->ub; i++){
        float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        pthread_mutex_lock(&mut);

        total_sum +=f1(x, struct_info->intensity);
	pthread_mutex_unlock(&mut);
      }
      break;

    case 2:
      for (int i=struct_info->lb; i<=struct_info->ub; i++){
        float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        pthread_mutex_lock(&mut);

        total_sum +=f2(x, struct_info->intensity);
	pthread_mutex_unlock(&mut);
      }
      break;

    case 3:
      for (int i=struct_info->lb; i<=struct_info->ub; i++){
        float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        pthread_mutex_lock(&mut);

        total_sum +=f3(x, struct_info->intensity);
	pthread_mutex_unlock(&mut);
      }
      break;

    case 4:
      for (int i=struct_info->lb; i<=struct_info->ub; i++){
        float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        pthread_mutex_lock(&mut);

        total_sum +=f4(x, struct_info->intensity);
	pthread_mutex_unlock(&mut);
      }
      break;
   }
}
else{
  switch(struct_info->functionid){
    case 1:
      for (int i=struct_info->lb; i<=struct_info->ub; i++){
        float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        struct_info->sum +=f1(x, struct_info->intensity);
      }
      break;

    case 2:
      for (int i=struct_info->lb; i<=struct_info->ub; i++){
        float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        struct_info->sum +=f2(x, struct_info->intensity);
      }
      break;

    case 3:
      for (int i=struct_info->lb; i<=struct_info->ub; i++){
        float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        struct_info->sum +=f3(x, struct_info->intensity);
      }
      break;

    case 4:
      for (int i=struct_info->lb; i<=struct_info->ub; i++){
        float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        struct_info->sum +=f4(x, struct_info->intensity);
      }
      break;
   }
   }
//total_sum = 2;
  return NULL;
}

int main (int argc, char* argv[]) {

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  if (argc < 7) {
    cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync>"<<endl;
    return -1;
  }

  float result;
  int functionid = atoi(argv[1]);
  float a = atof(argv[2]);

  float b = atof(argv[3]);
  int n = atoi(argv[4]);
  int intensity = atoi(argv[5]);

  float mid = ((b - a) / n );
  int nbthreads = atoi(argv[6]);

  string sync = argv[7];
  float sum[nbthreads];
  int computations = n/nbthreads;
  struct my_struct struct_info[nbthreads];
  pthread_t thread[nbthreads];

  if(0 == sync.compare("iteration")){
    pthread_mutex_init(&mut, NULL);
  }
int j = 0;

for(int i=0;i<nbthreads;i++){
if(j<n){
    struct_info[i].a = a;

    struct_info[i].b = b;
    struct_info[i].functionid = functionid;
    struct_info[i].intensity = intensity;

    struct_info[i].lb = j;
    struct_info[i].mid = mid;
    struct_info[i].sum = 0;
    struct_info[i].sync = sync;

    if((i+1) >= nbthreads){

      struct_info[i].ub = n - 1;

    }

    
   else{

     struct_info[i].ub = j + (computations-1);

    }  

    pthread_create(&thread[i], NULL, numerical_integration, (void*) &struct_info[i]);
  }

j+=computations;

}
  for(int i=0; i < nbthreads; i+=1)
  {
    pthread_join(thread[i], NULL);
  }

  if(0 == sync.compare("iteration"))
   {
      pthread_mutex_destroy(&mut);
  }
  else
  {
    
    for(int i=0;i < nbthreads;i+=1)
     {
        total_sum+= struct_info[i].sum;
     }
  }

  cout<<mid * total_sum;
  

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;

  std::cerr<<elapsed_seconds.count()<<std::endl;

  return 0;
}
