#include <mpi.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char*argv[]) {

int rank, size;
//printf("program started");
MPI_Init(&argc, &argv);
//pritnf("Inside this");
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

MPI_Comm_size(MPI_COMM_WORLD, &size);



char myhostname[128] = "";          
gethostname(myhostname, sizeof(myhostname));
printf("I am process %d out of %d. I am running on %s\n", rank, size, myhostname);

MPI_Finalize();

return 0;

}

