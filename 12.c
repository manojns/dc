# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include "mpi.h"

int search(int a, int b, int c, int id, int p);
int f(int i);

int main(int argc, char *argv[])
{
	int a=1, b=2147483647, c=45, id, j, p;
	double wtime;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if(id == 0) {
		printf ("Search the integers from A to B for a value J such that F(J) = C\n");
		printf ("	A = %d\n", a);
		printf ("	B = %d\n", b);
		printf ("	C = %d\n", c);
	}

	wtime = MPI_Wtime();
	j = search(a, b, c, id, p);
	wtime = MPI_Wtime() - wtime;

	if(j != -1)
		printf("  Process %d found J = %d\n  Verify F(J) : %d\n", id, j, f(j));

	if(id == 0)
		printf("  Elapsed wallclock time is %g\n", wtime);

	MPI_Finalize();

	if(id == 0)
		printf("Normal end of execution.\n");
	return 0;
}

int search(int a, int b, int c, int id, int p)
{
	int fi, i, j=-1;
	for(i=a+id; 0<i && i<=b; i=i+p) {
		fi = f(i);
		if(fi == c) {
			j = i;
			break;
		}
	}
	return j;
}

int f(int i) {
	int i4_huge = 2147483647, j, k, value=i;
	for ( j = 1; j <= 5; j++ ) {
		k = value/127773;
		value = 16807*(value-k*127773) - k*2836;
		if(value <= 0)
			value = value+i4_huge;
	}
	return value;
}
