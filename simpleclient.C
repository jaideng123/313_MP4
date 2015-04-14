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
	return NULL;
}

int main(int argc, char * argv[]) {

  cout << "CLIENT STARTED:" << endl;

  cout << "Establishing control channel... " << flush;
  RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
  cout << "done." << endl;;

  /* -- Start sending a sequence of requests */

  string reply1 = chan.send_request("hello");
  cout << "Reply to request 'hello' is '" << reply1 << "'" << endl;

  string reply2 = chan.send_request("data Joe Smith");
  cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << endl;

  string reply3 = chan.send_request("data Jane Smith");
  cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << endl;

  string reply5 = chan.send_request("newthread");
  cout << "Reply to request 'newthread' is " << reply5 << "'" << endl;
  RequestChannel chan2(reply5, RequestChannel::CLIENT_SIDE);

  string reply6 = chan2.send_request("data John Doe");
  cout << "Reply to request 'data John Doe' is '" << reply6 << "'" << endl;

  string reply7 = chan2.send_request("quit");
  cout << "Reply to request 'quit' is '" << reply7 << "'" << endl;

  string reply4 = chan.send_request("quit");
  cout << "Reply to request 'quit' is '" << reply4 << "'" << endl;

  usleep(1000000);
}
