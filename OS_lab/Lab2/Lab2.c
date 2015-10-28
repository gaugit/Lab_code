/*
TEST semaphore and mutex
Consider binary semaphore
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<signal.h>
#define BUFFER_SIZE 100

typedef int buffer_item;
buffer_item buffer[BUFFER_SIZE];

// global variable, all threads can acess
int index_counter_in = 0,index_counter_out = 0;
void *thread_Insert(void *arg); // function for sending
void *thread_Remove(void *arg); // function for receiving
sem_t buff_sem; // semaphore
pthread_mutex_t mutx; // mutex
char thread1[]="Producer A";
char thread2[]="Producer B";

int main(int argc, char **argv)
{
  pthread_t t1, t2, t3;
  pthread_t arr_t[340];
  void *thread_result;
  int state1, state2,i=0;
  //int arr_val[340];
  char cons[340][12];
  
  state1 = pthread_mutex_init(&mutx, NULL);
  state2 = sem_init(&buff_sem, 0 ,0);
  
  //mutex initialization
  //semaphore initialization, first value = BUFFER_SIZE
  
  if(state1||state2!=0)
    puts("Error mutex & semaphore initialization!!!");
  
  // Create thread1, thread2, thread3
  pthread_create(&t1, NULL, thread_Insert, &thread1);
  pthread_create(&t2, NULL, thread_Insert, &thread2);
  
  for(i=0;i<atoi(argv[1]);i++)
  {
    sprintf(cons[i], "Consumer %d", i);
    //arr_val[i]=i;
    //pthread_create(&arr_t[i], NULL, thread_Remove,(void *)&arr_val[i]);
    pthread_create(&arr_t[i], NULL, thread_Remove,&cons[i]);
  }
  
  // Waiting thread to terminate
  pthread_join(t1, &thread_result);
  pthread_join(t2, &thread_result);
  
  for(i=0;i<atoi(argv[1]);i++)
  {
    pthread_join(arr_t[i], NULL);
  }

//printf("Terminate => %s, %s, %s!!!\n", (char*)&thread1, (char*)&thread2, (char*)&thread3);
//  printf("Final Index: %d\n", index_counter_in);
  sem_destroy(&buff_sem); // destroy semaphore
  pthread_mutex_destroy(&mutx); // destroy mutex
  return 0;
}

// Thread increases item
void *thread_Insert(void *arg)
{
  printf("Creating Thread: %s\n", (char*)arg);
  while(1)
  {
    if(index_counter_in<BUFFER_SIZE)
    {
      pthread_mutex_lock(&mutx);
      buffer[index_counter_in] = index_counter_in;
      printf("%s: INSERT item to BUFFER %d\n", (char*)arg, index_counter_in);
      index_counter_in++;
      pthread_mutex_unlock(&mutx);
      sem_post(&buff_sem); // semaphore to increse
      sleep(1);
    }
    else
    {
      index_counter_in=0;
      sleep(2);
    }
  }
}

// Thread decreases item
void *thread_Remove(void *arg)
{
  //printf("Creating Thread: %d\n", *((int*)arg));
  printf("Creating Thread: %s\n", (char*)arg);

  while(1)
  {
    if(index_counter_out<BUFFER_SIZE)
    {
      sem_wait(&buff_sem); //decrease index_counter
      pthread_mutex_lock(&mutx);
      sleep(1);
      buffer[index_counter_out] = 0;
      printf("%s: REMOVE item from BUFFER %d\n", (char*)arg, index_counter_out);
      //printf("%d: REMOVE item from BUFFER %d\n", *((int*)arg), index_counter_out);
      index_counter_out++;
      pthread_mutex_unlock(&mutx);
      sleep(1);
    }
    else
    {
      index_counter_out=0;
      sleep(2);
    }
  }
}