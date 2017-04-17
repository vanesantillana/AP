#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main(void)
{
    MPI_Init(NULL, NULL);
    int id_procesos; 
    int num_procesos; 
    MPI_Comm_rank(MPI_COMM_WORLD, &id_procesos);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procesos);

    const int limit = 10;
    int cont = 0;
    int emisor = (id_procesos + 1) % 2;
    while (cont < limit) {
        if (id_procesos == cont % 2) { 
          cont++;
          MPI_Send(&cont, 1, MPI_INT, emisor, 0, MPI_COMM_WORLD);
          printf("%d Envio  :%d   con contador:%d \n", id_procesos, emisor, cont);
        } else {
          MPI_Recv(&cont, 1, MPI_INT, emisor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          printf("%d Recibo :%d   con contador:%d \n", id_procesos, emisor, cont);
        }
    }
    MPI_Finalize();
    return 0;

}