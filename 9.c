#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define N 1E8
#define d 1E-8

int main (int argc, char* argv[])
{
	int rank, size, i, result=0, sum=0;
	double pi=0.0, begin=0.0, end=0.0, x, y;
    
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Barrier(MPI_COMM_WORLD);
    begin = MPI_Wtime();
	srand((int)time(0));
	for (i=rank; i<N; i+=size)
    {
		x=rand()/(RAND_MAX+1.0);
		y=rand()/(RAND_MAX+1.0);
		if(x*x+y*y<1.0)
			result++;
	}
	MPI_Reduce(&result, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	end = MPI_Wtime();
	if (rank==0)
	{
		pi=4*d*sum;
		printf("np=%d\nTime=%fs\nPi=%0.4f\n", size, end-begin, pi);
	}
	MPI_Finalize();
	return 0;
}
