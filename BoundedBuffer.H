#include "semaphore.H"
#include <vector>
using namespace std;

class Item
{
public:
	Item();
	Item(int d, char p);
	~Item();
	int getData() { return data; }
	void setData(int x) { data = x; }
	char getPerson() { return person; }
	void setPerson(char c) { person = c; }
private:
	int data;
	char person; //identifier for the person who the item belongs to
};

class BoundedBuffer
{
public:
	BoundedBuffer(int size);
	~BoundedBuffer();
	void* addToBuffer(void* item);
	void* removeFromBuffer(void *item);
	int getMaxSize() { return maxSize; }
	void setMaxSize(int x) { maxSize = x; }
private:
	vector<Item> buffer;
	int maxSize = 0; //maximum size the buffer can be
};