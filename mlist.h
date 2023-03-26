#ifndef mlist_h
#define mlist_h

#include <stdio.h>

typedef struct item item;
typedef struct mlist mlist;

struct item
{
	struct item* next;
	struct item* previous;
	void* val;
};
struct mlist
{
	struct item* first;
	struct item* last;
	int count;
};

struct mlist* makeList();
void deleteList(mlist* listStart);
item* addItem(mlist* listPtr, void* val);
void deleteItem(mlist* lis, item* itm);
void printItem(item* itm);
void printlist(mlist* lis);
item* getItem(mlist* lis, void* indx);
item* firstPassingItem(mlist* lis, int(*testingFunc)(void*, void*), void* args);
void doForEach(mlist* lis, void(*func)(void*, void*), void* args);

void* pop(mlist* lis);


#endif