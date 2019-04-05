#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>


extern float f1(float x, int intensity);
extern float f2(float x, int intensity);
extern float f3(float x, int intensity);
extern float f4(float x, int intensity);

  
int function_id,a,b,n,intensity;
float x,p=0,sub,t;
clock_t s, e, caltime; 
double my_time; 





int main (int argc, char* argv[]) {


  s=clock();


  if (argc < 6) {
    fprintf(stderr, "usage: %s <functionid> <a> <b> <n> <intensity>", argv[0]);
    return -1;
  }
  

int i; 
  sscanf(argv[1],"%d",&function_id);
  sscanf(argv[2],"%d",&a);
  sscanf(argv[3],"%d",&b);
  sscanf(argv[4],"%d",&n);
  sscanf(argv[5],"%d",&intensity);

  sub = b-a;
  t = sub/n;
 
 
for (i=0;i<=n-1;i++)
{
 x = a +( (i + .5 ) * t);

      switch(function_id)
      {

    case 1: p = p + f1(x,intensity);
        break;
    case 2: p = p + f2(x,intensity);
        break;
    case 3: p = p + f3(x,intensity);
        break;
    case 4: p = p + f4(x,intensity);
        break;
    default:printf("wrong F_id");
    break;

      }
}
    p = p*t;


     e=clock(); 
     fprintf(stdout, "%f\n",p);
     caltime=(double)e-s;
     my_time=caltime/CLOCKS_PER_SEC; 
     fprintf(stderr, "%f\n",my_time);

  return 0;
}
