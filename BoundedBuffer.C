#include "BoundedBuffer.H"
using namespace std;

Item::Item(char p, string m){
	person = p;
	message = m;
}

Item::Item(const Item& other){
	data = other.data;
	person = other.person;
	message = other.message;
}

BoundedBuffer::BoundedBuffer(int size){
	//
	maxSize = size;
}

BoundedBuffer::~BoundedBuffer(){
}

void BoundedBuffer::add(Item item){
	s.P();
	if(requestBuffer.size() < maxSize){
		requestBuffer.push_back(item);
		s.V();
	}
	else{
		s.V();
		while (true){
			if(requestBuffer.size() < maxSize){
			s.P();
			requestBuffer.push_back(item);
			s.V();
			break;
			}
		}
	}
}

Item BoundedBuffer::removeFromRequestBuffer(){
	s.P();
	Item item(requestBuffer[0].getData(), requestBuffer[0].getPerson(), requestBuffer[0].getMessage());
	requestBuffer.erase(requestBuffer.begin());
	s.V();
	return item;
}

void BoundedBuffer::addToResponseBuffer(Item item){
	s.P();
	responseBuffer.push_back(item);
	s.v();
}

Item BoundedBuffer::removeFromResponseBuffer(){
	s.P();
	Item item(responseBuffer[0].getData(), responseBuffer[0].getPerson(), responseBuffer[0].getMessage());
	responseBuffer.erase(responseBuffer.begin());
	s.v();
	return item;
}