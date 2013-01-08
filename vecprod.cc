#include<iostream>
#include<vector>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<cassert>
#define MAX_SIZE 99999
#define DEBUG
#define MAX_THREADS 300

using namespace std;

int NUM_THREADS, SIZE_VECTORS; 
double a[MAX_SIZE],b[MAX_SIZE],c[MAX_SIZE];
double result; 
pthread_t threads[MAX_THREADS]; 
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
double s_result;

void dot_prod_seq()
{
  for(int i = 0; i < SIZE_VECTORS; i++){
    s_result += (a[i] * b[i]);
   
  }

}
void *dotprod(void* thread_id){
  long tid; 
  tid = (long) thread_id;
  int start = tid * (SIZE_VECTORS/NUM_THREADS);
  int end = start + (SIZE_VECTORS/NUM_THREADS);

  for(int i = start; i < end; i++){
    
    c[i] = (a[i] * b[i]);

  }

  pthread_mutex_lock(&m1);

  for(int i = start; i < end; i++)
      result += c[i];
  pthread_mutex_unlock(&m1);
  // cout<<result<<endl;  
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{


  int status = 0; 
  if(argc != 3){
    cout<<"Usage:    "<<endl;
    cout<<"          ./vecprod <NUM_OF_THREADS> <SIZE_VECTORS>  "<<endl;
    cout<<endl;
  }
  else{
    NUM_THREADS = atoi(argv[1]);
    SIZE_VECTORS = atoi(argv[2]);
    assert(NUM_THREADS <= SIZE_VECTORS);
    assert(SIZE_VECTORS < MAX_SIZE);
    assert(NUM_THREADS < MAX_THREADS );
    assert(SIZE_VECTORS % NUM_THREADS == 0);
  }



  for(int i = 0; i < SIZE_VECTORS; i++){
    a[i] = (double)(rand()%10)/5;
    b[i]  =(double) (rand()%10)/3; 

  }

  /*Thread Creation*/

  for(long i = 0; i < NUM_THREADS; i++){

    status = pthread_create(&threads[i],NULL,dotprod, (void *)i);
   
    if(status){
      cout<<"Error in creating thread " <<i<<endl;
    }
  }
  dot_prod_seq();


  for (int loop=0; loop<NUM_THREADS; loop++) {
    status = pthread_join(threads[loop], NULL);
  } 
  //  cout<<result<<" "<<s_result<<endl; 
  pthread_exit(NULL);

 
 
  return 0;
}
