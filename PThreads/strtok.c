#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <stdlib.h>

int thread_count;
sem_t sems[2]={0};
int MAX=1000;

void* Tokenize(void* rank){
  long my_rank = (long) rank;
  int count;
  int next = (my_rank + 1) % thread_count;
  char *fg_rv;

  char *saveptr_p;
  char my_line[MAX];
  char *my_string;
  sem_wait(&sems[my_rank]);
  fg_rv = fgets(my_line, MAX, stdin);
  sem_post(&sems[next]);

  while(fg_rv != NULL){
    printf("THread %ld > linea = %s", my_rank, my_line);
    count = 0;
    my_string = strtok_r(my_line," \t\n", &saveptr_p);
    while(my_string != NULL){
      count++;
      printf("Thread %ld > string %d = %s\n", my_rank, count, my_string);
      my_string = strtok_r(NULL, " \t\n", &saveptr_p);
    }
    sem_wait(&sems[my_rank]);
    fg_rv = fgets(my_line, MAX, stdin);
    sem_post(&sems[next]);
  }
  return NULL;
}


int main(int argc, char* argv[]){
  	long       thread;
  	pthread_t* thread_handles;

  	thread_count = strtol(argv[1], NULL, 10);
  	thread_handles = malloc(thread_count*sizeof(pthread_t));
  	sem_post(&sems[0]);
  	for (thread = 0; thread < thread_count; thread++)
      	pthread_create(&thread_handles[thread], NULL,
    	Tokenize, (void*) thread);

  	for (thread = 0; thread < thread_count; thread++)
      	pthread_join(thread_handles[thread], NULL);

    free(thread_handles);
  	return 0;
}