#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

void parametros(char* prog_name) {
   fprintf(stderr, "parametros: %s thread_count number_tosses\n", prog_name); 
   exit(0);
}

double randomValor() {
   double randomValor_value;
   srand ( time ( NULL));
   randomValor_value = (double)rand()/RAND_MAX*2.0-1.0;
   return randomValor_value;
}

int main(int argc, char* argv[]) {
   int number_tosses, number_in_circle;
   int thread_count;
   double x, y, distancia;

   if (argc != 3) parametros(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   number_tosses = strtoll(argv[2], NULL, 10);
   if (thread_count < 1 || number_tosses < 1) parametros(argv[0]);

   number_in_circle =0;
   
#  pragma omp parallel for num_threads(thread_count)\
   reduction(+: number_in_circle) private(x, y, distancia)
   for (int i = 0; i < number_tosses; i++) {
      x = randomValor();
      y = randomValor();
      distancia = x*x + y*y;

      if (distancia <= 1) {
         number_in_circle += 1;
      }
   }
   double pi = 4*number_in_circle/((double) number_tosses);
   printf("pi = %.14f\n", pi);
   return 0;
}  

