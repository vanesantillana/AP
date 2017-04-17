#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int thread_count;
int flag = 0;
double sum = 0, my_sum = 0;
long long n =100000;
pthread_mutex_t mutex;

//Calculo de Pi
void* Thread_sum(void* rank){
  long my_rank = (long) rank;
  double factor;
  long long i;
  long long my_n = n/thread_count;
  long long my_first_i = my_n * my_rank;
  long long my_last_i = my_first_i + my_n;

  if(my_first_i % 2 == 0) factor = 1.0;
  else factor = -1.0;

  for(i = my_first_i; i < my_last_i; i++, factor = -factor){
    sum += factor/(2*i+1);
  }

  return NULL;

}


void* Thread_sum_busyWaiting(void* rank){
  long my_rank = (long) rank;
  double factor;
  long long i;
  long long my_n = n/thread_count;
  long long my_first_i = my_n * my_rank;
  long long my_last_i = my_first_i + my_n;

  if(my_first_i % 2 == 0) factor = 1.0;
  else factor = -1.0;

  for(i = my_first_i; i < my_last_i; i++, factor = -factor){
    my_sum += factor/(2*i+1);
  }

  while(flag != my_rank);
  sum += my_sum;
  flag = (flag+1) % thread_count;

  return NULL;

}

void* Thread_sum_Mutex(void* rank){
  long my_rank = (long) rank;
  double factor;
  long long i;
  long long my_n = n/thread_count;
  long long my_first_i = my_n * my_rank;
  long long my_last_i = my_first_i + my_n;

  if(my_first_i % 2 == 0) factor = 1.0;
  else factor = -1.0;

  for(i = my_first_i; i < my_last_i; i++, factor = -factor){
    my_sum += factor/(2*i+1);
  }

  pthread_mutex_lock(&mutex);
  sum += my_sum;
  pthread_mutex_unlock(&mutex);

  return NULL;

}
int main(int argc, char* argv[]){
  long       thread;
  pthread_t* thread_handles;
  clock_t time;

  thread_count = strtol(argv[1], NULL, 10);
  thread_handles = malloc(thread_count*sizeof(pthread_t));

  time = clock();
	for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
      Thread_sum_Mutex, (void*) thread);

  for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
	time = clock() - time;
	printf("La suma es %lf\n", sum);
  printf("Tiempo:%lf\n", (((float)time)/CLOCKS_PER_SEC));
  return 0;
}