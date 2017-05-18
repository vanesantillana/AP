#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int thread_count;

void parametros(char prog_name[]) {
   fprintf(stderr, "parametros: %s <bin_count> <min_meas> <max_meas> <data_count> <thread_count>\n",prog_name);
   exit(0);
}  


void get_args(char* argv[], int* bin_count_p, float* min_meas_p, float* max_meas_p, int* data_count_p, int* thread_count) {
   *bin_count_p = strtol(argv[1], NULL, 10);
   *min_meas_p = strtof(argv[2], NULL);
   *max_meas_p = strtof(argv[3], NULL);
   *data_count_p = strtol(argv[4], NULL, 10);
   *thread_count = strtol(argv[5], NULL, 10);
}  

void generarData(float min_meas, float max_meas, float data[], int data_count) {
   int i;

   srandom(0);
   #pragma omp parallel for num_threads(thread_count) \
      default(none) shared(data, min_meas, max_meas, data_count)
   for (i = 0; i < data_count; i++) 
      data[i] = min_meas + (max_meas - min_meas) * random() / ((double) RAND_MAX);
      
}  

void generarBins(float min_meas, float max_meas, float bin_maxes[], int bin_counts[], int bin_count) {
   float bin_width;
   int   i;

   bin_width = (max_meas - min_meas)/bin_count;

   #pragma omp parallel for num_threads(thread_count) \
      default(none) \
      shared(min_meas, max_meas, bin_maxes, bin_counts, bin_count, bin_width) \
      private(i)
   for (i = 0; i < bin_count; i++) {
      bin_maxes[i] = min_meas + (i+1)*bin_width;
      bin_counts[i] = 0;
   }

}  

int llenarBin(float   data, float  bin_maxes[], int bin_count, float  min_meas) {
   int bottom = 0, top =  bin_count-1;
   int mid;
   float bin_max, bin_min;

   while (bottom <= top) {
      mid = (bottom + top)/2;
      bin_max = bin_maxes[mid];
      bin_min = (mid == 0) ? min_meas: bin_maxes[mid-1];
      if (data >= bin_max) 
         bottom = mid+1;
      else if (data < bin_min)
         top = mid-1;
      else
         return mid;
   }
   exit(-1);
} 

void imprimir(float  bin_maxes[], int bin_counts[], int bin_count, float  min_meas) 
{
   int i, j;
   float bin_max, bin_min;

   for (i = 0; i < bin_count; i++) {
      bin_max = bin_maxes[i];
      bin_min = (i == 0) ? min_meas: bin_maxes[i-1];
      printf("%.3f-%.3f:\t", bin_min, bin_max);
      for (j = 0; j < bin_counts[i]; j++)
         printf("≡");
      printf("\n");
   }
} 


int main(int argc, char* argv[]) {
   int bin_count, i, bin;
   float min_meas, max_meas;
   float* bin_maxes;
   int* bin_counts;
   float* data;
   int data_count;

   /* Check parametros*/
   if (argc != 6) parametros(argv[0]);
   get_args(argv, &bin_count, &min_meas, &max_meas, &data_count, &thread_count);

   bin_maxes = malloc(bin_count*sizeof(float)); // array de los float
   bin_counts = malloc(bin_count*sizeof(int));  // array de los int 
   data = malloc(data_count*sizeof(float));

   /* DATOS */
   generarData(min_meas, max_meas, data, data_count);

   /* CONTENEDORES*/
   generarBins(min_meas, max_meas, bin_maxes, bin_counts, bin_count);

   /* Cuenta los datos para cada bin */
   #pragma omp parallel for num_threads(thread_count) default(none) \
      shared(data_count, data, bin_maxes, bin_count, min_meas, bin_counts) \
      private(bin, i)
   for (i = 0; i < data_count; i++) {
      bin = llenarBin(data[i], bin_maxes, bin_count, min_meas);
      #pragma omp critical
      bin_counts[bin]++;
   }

   imprimir(bin_maxes, bin_counts, bin_count, min_meas);

   free(data);
   free(bin_maxes);
   free(bin_counts);
   return 0;

} 

