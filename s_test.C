#include "semaphore.h"
#include <iostream>
#include <pthread.h>

using namespace std;
pthread_t t0;
pthread_t t1;
pthread_t t2;
pthread_t t3;
//Semaphore s(1)
void *inc(void *param){
	int *x_ptr = (int *)param;
	for (int i = 0; i < 100000; ++i)
	{
		(*x_ptr)++;
	}
	cout<<"x increment finished\n";
}
void *dec(void *param){
	int *x_ptr = (int *)param;
	
	cout<<"x decrement finished\n";
}
int main(){
	//Semaphore sem(1);
	int number = 0;
	pthread_create(&t0, NULL, inc, &number);
	pthread_create(&t1, NULL, inc, &number);
	pthread_create(&t2, NULL, inc, &number);
	pthread_create(&t3, NULL, inc, &number);
	pthread_join(t0, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	cout<<number<<endl;
}