#include "BoundedBuffer.H"
using namespace std;

Item::Item(char p, string m){
	person = p;
	message = m;
}

BoundedBuffer::BoundedBuffer(int size, Semaphore* sem){
	s = sem;
	maxSize = size;
}

BoundedBuffer::~BoundedBuffer(){
}

void BoundedBuffer::add(Item item){
	s->P();
	if(requestBuffer.size() < maxSize){
		requestBuffer.push_back(item);
		s->V();
	}
	else{
		s->V();
		while (true){
			if(requestBuffer.size() < maxSize){
			s->P();
			requestBuffer.push_back(item);
			s->V();
			break;
			}
		}
	}
}

Item BoundedBuffer::removeFromRequestBuffer(){
	s->P();
	Item item(requestBuffer[0].getPerson(), requestBuffer[0].getMessage());
	requestBuffer.erase(requestBuffer.begin());
	s->V();
	return item;
}

void BoundedBuffer::addToResponseBuffer(Item item){
	s->P();
	responseBuffer.push_back(item);
	s->V();
}

Item BoundedBuffer::removeFromResponseBuffer(){
	s->P();
	Item item( responseBuffer[0].getPerson(), responseBuffer[0].getMessage());
	responseBuffer.erase(responseBuffer.begin());
	s->V();
	return item;
}