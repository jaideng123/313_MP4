#include "semaphore.H"
#include <iostream>
#include <pthread.h>

using namespace std;
pthread_t t0;
pthread_t t1;
pthread_t t2;
pthread_t t3;
Semaphore s(1);
void *inc(void *param){
	int *x_ptr = (int *)param;
	for (int i = 0; i < 100000; ++i)
	{
		s.P();
		(*x_ptr)++;
		s.V();
	}
	cout<<"x increment finished\n";
}

/*int main(int argc, char ** argv){
    int c;
	int index;
	int n = 8; //number of data requests per person
	int b = 10000; //size of bounded buffer in requests
	int w = 3; //number of worker threads
	while ((c = getopt (argc, argv, "b:s:")) != -1) {
		switch(c) {
		case 'b':
			b = atoi(optarg);
			break;
		case 's':
			s = atoi(optarg);
			break;
		case '?':
			return 1;
		default:
			abort();
		}
	}
	
//for getopt failures
	for(index = optind; index < argc; ++index){
		printf("Non-option argument %s\n", argv[index]);
	}
	return 0;
}
*/

int main(){
	//Semaphore sem(1);
	int number = 0;
	BoundedBuffer buffer(3);
	Item item1(1, '1');
	Item item2(2, '2');
	Item item3(3, '3');
	pthread_create(&t0, NULL, buffer.addToBuffer, item1);
	pthread_create(&t1, NULL, buffer.addToBuffer, item2);
	pthread_create(&t2, NULL, buffer.addToBuffer, item3);
	pthread_join(t0, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	/*pthread_create(&t0, NULL, inc, &number);
	pthread_create(&t1, NULL, inc, &number);
	pthread_create(&t2, NULL, inc, &number);
	pthread_create(&t3, NULL, inc, &number);
	pthread_join(t0, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);*/
	cout<<number<<endl;
}