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
void *request(void *param){
	Arguments *args = (Arguments *)param;
	for(int i = 0; i < 10; ++i){
		Item item(args->id,"data Joe Smith");
		args->b->add(item);
	}
  args->b->finished();
}
void *worker(void *param){
  BoundedBuffer* b = (BoundedBuffer *)param;
  while(b->numFinished != 3 && b->getSize() != 0){
    Item i = b->remove();
    RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
    i.setData(chan.send_request(i.getMessage()));
  }
}

int main(int argc, char * argv[]) {
  pid_t parent = getpid();
  pid_t pid = fork();
  vector<pthread_t> clients(3);
  vector<pthread_t> workers(3);

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
  Semaphore s(1);
  BoundedBuffer b(50,&s);
  Arguments arg1;
  arg1.id = 'j';
  arg1.b = &b;
  pthread_create(&clients[0], NULL, request, &arg1);
  Arguments arg2;
  arg2.id = 'l';
  arg2.b = &b;
  pthread_create(&clients[1], NULL, request, &arg2);
  Arguments arg3;
  arg3.id = 'g';
  arg3.b = &b;
  pthread_create(&clients[2], NULL, request, &arg3);
  for (int i = 0; i < clients.size(); ++i)
  {
    pthread_join(clients[i], NULL);
  }
  cout<<b.getSize()<<endl;

  usleep(1000000);
}
