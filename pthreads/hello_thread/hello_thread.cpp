#include <iostream>
#include <stdio.h>
#include <string>

struct my_thread{
//creating a structure for my_thread
	int num_threads;
	int c_thread;
};

void* f(void* p){
	my_thread *mthread = (my_thread*)p;
	printf("I am thread %d in %d \n", mthread->c_thread,mthread->num_threads);
 //std::cout<<"I am thread "<<mthread->c_thread<<" in "<<mthread->num_threads<<std::endl;
}

int main (int argc, char* argv[]) {

if (argc < 2) 
{
	std::cerr<<"usage: "<<argv[0]<<" <nbthreads>"<<std::endl;
	return -1;
}

int n_threads = std::stoi(argv[1]);

//Setting the length of my structured thread equals to the number of threads
struct my_thread mthread[n_threads];


pthread_t nbthread[n_threads];

for(int i=0; i < n_threads; i+=1)
{
	mthread[i].num_threads = n_threads;
	mthread[i].c_thread = i; 
//create pthread
  	pthread_create(&nbthread[i], NULL, f, (void*) &mthread[i]);
}

for(int i=0; i < n_threads; i+=1)
{
//join with a terminated thread
	pthread_join(nbthread[i], NULL);
}
  
	return 0;
}
