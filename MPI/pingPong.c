#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 1;

int main(void)
{
    char num[MAX_STRING];
    int num_proceso;
    int id_proceso;
	
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD , &num_proceso);
    MPI_Comm_rank(MPI_COMM_WORLD , &id_proceso );

    if(id_proceso != 0)
    {
        sprintf(num," Salgo desde el proceso %d al %d!",id_proceso,num_proceso);
        MPI_Send(num,strlen(num)+1, MPI_CHAR , 0, 0, MPI_COMM_WORLD);        
    }
    else
    {
        printf("Salgo dede el proceso %d al %d!\n",id_proceso,num_proceso);
        for(int i=1; i< num_proceso;i++)
        {
            MPI_Recv(num,MAX_STRING,MPI_CHAR,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf("%s\n",num);
        }
    }
    
    MPI_Finalize();
    return 0;

}
