#include "BoundedBuffer.H"
using namespace std;

Item::Item(char p, string m){
	person = p;
	message = m;
}

BoundedBuffer::BoundedBuffer(int size, Semaphore* sem){
	s = sem;
	maxSize = size;
	numFinished = 0;
}

BoundedBuffer::~BoundedBuffer(){
}

void BoundedBuffer::add(Item item){
	s->P();
	if(buffer.size() < maxSize){
		buffer.push_back(item);
		s->V();
	}
	else{
		s->V();
		while (true){
			if(buffer.size() < maxSize){
			s->P();
			buffer.push_back(item);
			s->V();
			break;
			}
		}
	}
}

Item BoundedBuffer::remove(){
	s->P();
	Item item(buffer[0].getPerson(),buffer[0].getMessage());
	buffer.erase(buffer.begin());
	s->V();
	return item;
}