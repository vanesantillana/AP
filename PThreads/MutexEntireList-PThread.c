#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define  n_member 80000
#define  n_insert 10000
#define  n_delete 10000

struct list_node_s{
  int data;
  struct list_node_s* next;
};

int Member(int value, struct list_node_s* head_p){
  struct list_node_s* curr_p = head_p;

  while(curr_p != NULL && curr_p->data < value)
    curr_p = curr_p->next;

  if(curr_p == NULL || curr_p->data > value)
    return 0;
  else 
    return 1;
  
}

int Insert(int value, struct list_node_s** head_p){
  struct list_node_s* curr_p = *head_p;
  struct list_node_s* pred_p = NULL;
  struct list_node_s* temp_p;

  while(curr_p != NULL && curr_p->data < value){
    pred_p = curr_p;
    curr_p = curr_p->next;
  }

  if(curr_p == NULL || curr_p->data > value){
    temp_p = malloc(sizeof(struct list_node_s));
    temp_p->data = value;
    temp_p->next = curr_p;
    if(pred_p == NULL)
      *head_p = temp_p;
    else pred_p->next = temp_p;
    return 1;
  }
  else return 0;
}

int Delete(int value, struct list_node_s** head_p){
  struct list_node_s* curr_p = *head_p;
  struct list_node_s* pred_p = NULL;

  while(curr_p != NULL && curr_p->data < value){
    pred_p = curr_p;
    curr_p = curr_p->next;
  }

  if(curr_p != NULL && curr_p->data == value){
    if(pred_p == NULL){
      *head_p = curr_p->next;
      free(curr_p);
    }
    else{
      pred_p->next = curr_p->next;
      free(curr_p);
    }
    return 1;
  }
  else
    return 0;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct list_node_s* list;

void* Tabla(void* rank){
  long my_rank = (long) rank;
  int i;
  srand(time(NULL));

  for(i = 0; i < n_member; i++) {
    pthread_mutex_lock(&mutex);
    Insert(rand()%100,&list);
    pthread_mutex_unlock(&mutex);
  }
  
  for(i = 0; i < n_insert; i++) {
    pthread_mutex_lock(&mutex);
    Member(rand()%100,list);
    pthread_mutex_unlock(&mutex);
  }
  
  for(i = 0; i < n_delete; i++) {
    pthread_mutex_lock(&mutex);
    Delete(rand()%100,&list);
    pthread_mutex_unlock(&mutex);
  }
  
  return NULL;
}


int main(int argc, char* argv[]){

    long thread;
    double thread_count;
    pthread_t *thread_handles;
    clock_t time;
    struct list_node_s* list = NULL;
    pthread_rwlock_init(&mutex,NULL);
    thread_count = strtol(argv[1],NULL,10);
    thread_handles = malloc(thread_count* sizeof(pthread_t));
    time = clock();
    for(thread=0;thread<thread_count;thread++)
        pthread_create(&thread_handles[thread],NULL,
        Tabla,(void*)thread);
    
    for(thread=0;thread<thread_count;thread++)
        pthread_join(thread_handles[thread],NULL);

    time = clock() - time;
    printf("Tiempo:%lf\n", (((float)time)/CLOCKS_PER_SEC));
    
    return 0;
}