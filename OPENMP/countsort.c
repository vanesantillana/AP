#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>


void generarDatos(int * a, int n) {
   int i;
   for (i = 0; i < n; ++i) {
      a[i] = random() % 100;
   }
}

int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}


void imprimir(int * a, int n) {
   int i;
   for (i = 0; i < n; ++i) {
      printf("%d ", a[i]);
   }
   printf("\n");
}


int main(int argc, char* argv[]) {
   int number_tosses, number_in_circle;
   int thread_count, i, j, n, count;
   srandom(0);
   thread_count = strtol(argv[1], NULL, 10);
   n = strtol(argv[2], NULL, 10);
   int * a = malloc(n* sizeof(int));
   generarDatos(a, n);
   //imprimir(a, n);

   //count_sort 
   int * temp = malloc(n* sizeof(int));
   double start = omp_get_wtime();
#pragma omp parallel for num_threads(thread_count) \
   default(none) private(i, j, count) shared(a, n, temp, thread_count)
      for (i = 0; i < n; i++) {
         count = 0;
         for (j = 0; j < n; j++)
            if (a[j] < a[i])
               count++;
            else if (a[j] == a[i] && j < i)
               count++;
         temp[count] = a[i];
      }
   memcpy ( a , temp, n * sizeof(int));

   //usando qsort
   //qsort(a, n, sizeof(int), cmpfunc);

   double finish = omp_get_wtime();
   free(temp );
   printf("Tiempo %e seg\n", finish - start);
   //imprimir(a, n);
   return 0;
}  

/*
a)cuáles variables deben ser privado y que debe ser compartida? privadas i,j, count compartidas a, n, temo, thread_count
b)en la parte anterior, hay alguna dependencia de bucle dependiente? No,porq no hay dependencia entre las iteraciones,
c)¿Podemos paralelizar la llamada a memcpy? Copia temp al array a,  si se puede paralelizar, pero n deberia poder cambiar de tamaño y loas arrays deben mandarce con el inicio .
   int inicio=omp_get_num_thread()*n/thread_count;
   memcpy ( &a[inicio] , &temp[inicio], n/thread_count * sizeof(int));
e) qsort          nlog (n) 
   count_sort     n²
*/