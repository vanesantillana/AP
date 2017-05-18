#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define bin_count 5;

int main(int argc, char** argv){

    int thread_count = strtol(argv[1],NULL,10);
    int data_count[20]={1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3, 4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9};
    
	# pragma omp parallel num_threads(thread_count)
	Hello();
	return 0;
}