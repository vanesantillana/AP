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
  pthread_mutex_t mutex;
};


int Member(int value, struct list_node_s* head_p){
  
  struct list_node_s* temp_p;
  pthread_mutex_lock(&(head_p->mutex));
  temp_p=head_p;
  while(temp_p != NULL && temp_p->data < value){
    if (temp_p->next != NULL)
      pthread_mutex_lock(&(temp_p->next->mutex));
    if (temp_p == head_p)
      pthread_mutex_unlock(&(head_p->mutex));
    pthread_mutex_unlock(&(temp_p->mutex));
    temp_p = temp_p->next;
  }

  if (temp_p == NULL || temp_p->data > value) {
    if (temp_p == head_p)
      pthread_mutex_unlock(&(head_p->mutex));
    if (temp_p != NULL)
      pthread_mutex_unlock(&(temp_p->mutex));
    return 0;
  } 
  else {
      if (temp_p == head_p)
        pthread_mutex_unlock(&(head_p->mutex));
      pthread_mutex_unlock(&(temp_p->mutex));
      return 1;
  }
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
  else{
    return 0;
  }
}

pthread_rwlock_t rwlock;
struct list_node_s* list;

void* Tabla1(void* rank){
  long my_rank = (long) rank;
  int i;
  srand(time(NULL));

  for(i = 0; i < n_member; i++) {
    Insert(rand()%100,&list);
  }
  
  for(i = 0; i < n_insert; i++) {
    Member(rand()%100,&list);
  }
  
  for(i = 0; i < n_delete; i++) {
    Delete(rand()%100,&list);
  }
  
  return NULL;
}


int main(int argc, char* argv[]){

    long thread;
    int thread_count;
    pthread_t *thread_handles;
    clock_t time;
    struct list_node_s* list = NULL;
    pthread_rwlock_init(&rwlock,NULL);
    thread_count = strtol(argv[1],NULL,10);
    thread_handles = malloc(thread_count* sizeof(pthread_t));
    time = clock();
    for(thread=0;thread<thread_count;thread++)
        pthread_create(&thread_handles[thread],NULL,
        Tabla1,(void*)thread);
    
    for(thread=0;thread<thread_count;thread++)
        pthread_join(thread_handles[thread],NULL);

    time = clock() - time;
    printf("Tiempo:%lf\n", (((float)time)/CLOCKS_PER_SEC));
    
    return 0;
}