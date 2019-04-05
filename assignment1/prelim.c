#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


void checkName(int x){
	if(x==-1){
		printf("Error");
	}
}

int main () {

	char name[256];
	int hostname;

	hostname=gethostname(name, sizeof(name));
	checkName(hostname);

	printf("Hostname :  %s \n", name);

  return 0;
}
