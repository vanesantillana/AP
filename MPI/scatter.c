#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
 
int main(int argc, char * argv[]) {
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	if(my_rank==0)
	{
	datos=(int*)malloc(sizeof(int)*TAMA*p);
	inicializa(my_rank,datos,TAMA*p);
	}
	MPI_Scatter(datos,TAMA,MPI_INT,mis_datos,TAMA,MPI_INT,root,MPI_COMM_WORLD)
	printf("Datos recibidos por proceso %d son:\n", my_rank);
	escribe(mis_datos,TAMA);
	if(my_rank==0)
	{
	free(datos);
	}
	MPI_Finalize();
}