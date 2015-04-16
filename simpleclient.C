/* 
    File: simpleclient.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2013/01/31

    Simple client main program for MP3 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <cstring>
#include <algorithm>  

#include <errno.h>
#include <unistd.h>

#include "reqchannel.H"
#include "BoundedBuffer.H"
#include <pthread.h>

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

struct Arguments{
	char id;
	BoundedBuffer* b;
	unsigned int numRequests;
  string channel;
  vector<BoundedBuffer*> buffers;
};
struct histogramArgs{
  int id;
  BoundedBuffer* b;
  int numWorkers;
};

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/
vector< vector<int> > hist(3);
template<typename T> T* getPointer(T& t) { return &t; }
void *request(void *param){
	Arguments *args = (Arguments *)param;
	for(int i = 0; i < args->numRequests; ++i){
		if (args->id == 'j'){
			Item item('j',"data Joe Smith");
			args->b->add(item);
		}
		else if(args->id == 'l'){
			Item item('l',"data Jane Smith");
			args->b->add(item);
		}
		else if(args->id == 'g'){
			Item item('g',"data John Doe");
			args->b->add(item);
		}
	}
  args->b->finished();
}
void *worker(void *param){
  Arguments* arg = (Arguments *)param;
  RequestChannel chan(arg->channel, RequestChannel::CLIENT_SIDE);
  while(arg->b->numFinished < 3 || arg->b->getSize() != 0){
    Item i = arg->b->remove();
    if(i.getMessage() != "NULL" && i.getPerson() != 'n'){
      i.setData(chan.send_request(i.getMessage()));
      cout << "\nResponse: " << i.getData() << " " << i.getPerson() << endl;
      if(i.getPerson() == 'j')
        hist[0][atoi(i.getData().c_str())]++;
      else if(i.getPerson() == 'l')
        hist[1][atoi(i.getData().c_str())]++;
      else if(i.getPerson() == 'g')
        hist[2][atoi(i.getData().c_str())]++;
    }
  }
  chan.send_request("quit");
}
void *histogram(void *param){
  histogramArgs* arg = (histogramArgs *)param;
  while(arg->b->getSize() != 0 || arg->b->numFinished < arg->numWorkers){
    Item i = arg->b->remove();
    if(i.getMessage() != "NULL" && i.getPerson() != 'n'){
      hist[arg->id][atoi(i.getData().c_str())]++;
    }
  }
}
void printHistograms(){
  unsigned int numStars = 0;
  for (int i = 0; i < 3; ++i)
  {
    numStars = 0;
    cout<<"Histogram: "<<i+1<<endl;
    for (int j = 0; j < 100; ++j)
    {
      cout<<j<<":";
      for (int k = 0; k < hist[i][j]; ++k){
        cout<<"*";
		++numStars;
	  }
      cout<<endl;
    }
	cout << "Size of Histogram " << i+1 << ": " << numStars << endl;
  }
}

int main(int argc, char * argv[]) {
  pid_t parent = getpid();
  pid_t pid = fork();
  if (pid == -1)
  {
    // error, failed to fork()
  } 
  else if(pid == 0){
    // we are the child
     char *arg[12];
    if(execv("./dataserver",argv) == -1)
      cout<<"Error, fork failed"<<endl;
    return -1;
  }
  
  int c;
  int index;
  int n = 10; //number of data requests per person
  int br = 15; //size of bounded buffer in requests
  int w = 3; //number of worker threads
  while ((c = getopt (argc, argv, "n:b:w:")) != -1) {
  switch(c) {
	case 'n':
		n = atoi(optarg);
		break;
	case 'b':
		br = atoi(optarg);
		break;
	case 'w':
		w = atoi(optarg);
		break;
	case '?':
		return 1;
	default:
		abort();
	}
  }
  cout << "n: " << n << " b: " << br << " w: " << w << endl;
  vector<pthread_t> clients(3);
  vector<pthread_t> workers(w);
  vector<int> temp(100);
  hist[0] = temp;
  hist[1] = temp;
  hist[2] = temp;
  RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
  Semaphore s(1);
  BoundedBuffer b(br,&s);
  Arguments arg1;
  arg1.id = 'j';
  arg1.b = &b;
  arg1.numRequests = n;
  pthread_create(&clients[0], NULL, request, &arg1);
  Arguments arg2;
  arg2.id = 'l';
  arg2.b = &b;
  arg2.numRequests = n;
  pthread_create(&clients[1], NULL, request, &arg2);
  Arguments arg3;
  arg3.id = 'g';
  arg3.b = &b;
  arg3.numRequests = n;
  pthread_create(&clients[2], NULL, request, &arg3);
  vector<Arguments> arr(3);
  vector<BoundedBuffer> histBuffers;
  vector<Semaphore> histSemaphores;
  for (int i = 0; i < 3; ++i)
  {
    histSemaphores.push_back(Semaphore(1));
    histBuffers.push_back(BoundedBuffer(br,&histSemaphores[i]));
  }
  for (int i = 0; i < 3; ++i)
  {
    arr[i].channel = chan.send_request("newthread");
    arr[i].b = &b;
    for (int j = 0; j < 3; ++i)
    {
      arr[i].buffers.push_back(&histBuffers[j]);
    }
    pthread_create(&workers[i], NULL, worker, &arr[i]);
  }
  for (int i = 0; i < clients.size(); ++i)
  {
    pthread_join(clients[i], NULL);
  }
  for (int i = 0; i < 1; ++i)
  {
    pthread_join(workers[i], NULL);
  }
  chan.send_request("quit");
  usleep(1000000);
  printHistograms();
  
  //for getopt failures
	for(index = optind; index < argc; ++index){
		printf("Non-option argument %s\n", argv[index]);
	}
	return 0;
}
