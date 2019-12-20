# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include "mpi.h"

int main (int argc, char *argv[])
{
	MPI_Comm even_comm_id;
	MPI_Group even_group_id;
	int even_id, even_id_sum, even_p, *even_rank, i, id, j;
	MPI_Comm odd_comm_id;
	MPI_Group odd_group_id;
	int odd_id, odd_id_sum, odd_p, *odd_rank, p;
	MPI_Group world_group_id;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	if (id == 0) {
		printf ( "COMMUNICATOR_MPI - Master process:\n" );
		printf ( "  The number of processes is %d.\n", p );
	}
	printf("  Process %d says 'Hello, world!'\n", id);

	MPI_Comm_group(MPI_COMM_WORLD, &world_group_id);

	even_p = (p+1)/2;
	even_rank = (int *)malloc(even_p * sizeof(int));
	
	j=0;
	for (i=0; i<p; i=i+2)
	{
		even_rank[j] = i;
		j=j+1;
	}

	MPI_Group_incl(world_group_id, even_p, even_rank, &even_group_id);
	MPI_Comm_create(MPI_COMM_WORLD, even_group_id, &even_comm_id);

	odd_p = p/2;
	odd_rank = (int *)malloc(odd_p*sizeof(int));
	
	j=0;
	for(i = 1; i<p; i=i+2) {
		odd_rank[j] = i;
		j = j+1;
	}

	MPI_Group_incl(world_group_id, odd_p, odd_rank, &odd_group_id);
	MPI_Comm_create(MPI_COMM_WORLD, odd_group_id, &odd_comm_id);

	if(id%2 == 0) {
		MPI_Comm_rank(even_comm_id, &even_id);
		odd_id = -1;
	}
	else {
		MPI_Comm_rank(odd_comm_id, &odd_id);
		even_id = -1;
	}
	if(even_id != -1)
		MPI_Reduce(&id, &even_id_sum, 1, MPI_INT, MPI_SUM, 0, even_comm_id);
	if(even_id == 0) {
		printf ( "  Number of processes in even communicator = %d\n", even_p);
		printf ( "  Sum of global ID's in even communicator  = %d\n", even_id_sum);
	}
	if(odd_id != -1)
		MPI_Reduce(&id, &odd_id_sum,  1, MPI_INT, MPI_SUM, 0, odd_comm_id);
	if(odd_id == 0) {
		printf ( "  Number of processes in odd communicator  = %d\n", odd_p);
		printf ( "  Sum of global ID's in odd communicator   = %d\n", odd_id_sum);
	}
	
	MPI_Finalize ( );
	return 0;
}
