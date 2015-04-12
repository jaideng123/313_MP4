#include "semaphore.H"
#include <vector>
using namespace std;

class Item
{
public:
	Item();
	Item(int d, char p, string m);
	~Item();
	int getData() { return data; }
	void setData(int x) { data = x; }
	char getPerson() { return person; }
	void setPerson(char c) { person = c; }
	string getMessage() { return message; }
	void setMessage(string m) { message = m; }
private:
	int data;
	char person; //identifier for the person who the item belongs to
	string message;
};

class BoundedBuffer
{
public:
	BoundedBuffer(int size);
	~BoundedBuffer();
	void addToRequestBuffer(Item item);
	Item removeFromRequestBuffer();
	int getMaxSize() { return maxSize; }
	void setMaxSize(int x) { maxSize = x; }
	void addToResponseBuffer(Item item);
	Item removeFromResponseBuffer();
private:
	vector<Item> requestBuffer;
	vector<Item> responseBuffer;
	int maxSize = 0; //maximum size the buffer can be
};