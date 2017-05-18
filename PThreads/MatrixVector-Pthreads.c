#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int     thread_count;
int     n =8000,m=8000;
double* A;
double* x;
double* y;

void *Pth_mat_vect(void* rank) {
   long my_rank = (long) rank;
   printf("Esta el thread #%ld!\n", my_rank);
   int i, j;
   int local_m = m/thread_count; 
   int my_first_row = my_rank*local_m;
   int my_last_row = my_first_row + local_m - 1;

   for (i = my_first_row; i <= my_last_row; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++)
          y[i] += A[i*n+j]*x[j];
   }

   return NULL;
}

void ini_matrix(double A[], int m, int n) {
   int i, j;
   srand(time(NULL));  
   for (i = 0; i < m; i++) 
      for (j = 0; j < n; j++)
         A[i*n+j]=rand()%100;
}  

void ini_vector(double x[], int n) {
   int   i;
   srand(time(NULL));
   for (i = 0; i < n; i++) 
      x[i]=rand()%100;
}  

void Print_vector(double y[], int m) {
   int   i;
   for (i = 0; i < m; i++)
      printf("%4.1f ", y[i]);
   printf("\n");
}

int main(int argc, char* argv[]) {
   long       thread;
   pthread_t* thread_handles;
   clock_t time;

   thread_count = strtol(argv[1], NULL, 10);
   thread_handles = malloc(thread_count*sizeof(pthread_t));

   A = malloc(m*n*sizeof(double));
   x = malloc(n*sizeof(double));
   y = malloc(m*sizeof(double));
   
   ini_matrix(A, m, n);
   ini_vector(x, n);

   time = clock();
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
         Pth_mat_vect, (void*) thread);

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);

   ///Print_vector(y, m);
   time = clock() - time;
    printf("Tiempo:%lf\n", (((float)time)/CLOCKS_PER_SEC));

   free(A);
   free(x);
   free(y);
   free(thread_handles);

   return 0;
}  