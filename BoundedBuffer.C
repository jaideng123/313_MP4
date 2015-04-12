#include "BoundedBuffer.H"

Item::Item(){

}

Item::Item(int d, char p){
	data = d;
	person = p;
}

Item::Item(const Item& other){
	data = other.data;
	person = other.person;
}

Item::~Item(){
}

BoundedBuffer::BoundedBuffer(int size){
	maxSize = size;
}

BoundedBuffer::~BoundedBuffer(){
}

void *BoundedBuffer::addToBuffer(void *item){
	Item *temp = (Item *) item;
	if (buffer.size() < maxSize){
		s.P();
		buffer.push_back(*temp);
		s.V();
	}
}

void *BoundedBuffer::removeFromBuffer(void *item){
	s.P();
	((Item *)item)->setData(buffer[0].getData());
	((Item *)item)->setPerson(buffer[0].getPerson());
	buffer.erase(buffer.begin());
	s.V();
}