#include "mlist.h"
#include <malloc.h>

#define debugging

mlist* makeList()
{
	mlist* lis = (mlist*)malloc(sizeof(mlist));
	if (lis == NULL)
	{
		return(lis);
	}
	else
	{
		mlist tempLis;
		tempLis.first = NULL;
		tempLis.last = NULL;
		tempLis.count = 0;
		(*lis) = tempLis;
		return(lis);
	}
}
item* makeItem()
{
	item* itm = (item*)malloc(sizeof(item));
	item tempItem = { NULL, NULL, NULL };
	(*itm) = tempItem;
	return(itm);
}
item* addItem(mlist* lis, void* val)
{
	item* itmPtr = makeItem();
	(*itmPtr).val = val;
	if ((*lis).count == 0)
	{
		(*lis).last = itmPtr;
		(*lis).first = itmPtr;
	}
	else
	{
		(*(*lis).last).next = itmPtr;
#ifdef debugging
		* ((int*)((*(*lis).last).next)) += 0;
#endif
		(*itmPtr).previous = (*lis).last;
		(*lis).last = itmPtr;
	}
	//(*lis).avreg = ((*lis).avreg * (*lis).count + val) / ((*lis).count + 1);
	(*lis).count += 1;
#ifdef debugging
	* ((int*)((*itmPtr).val)) += 0;
	if ((*lis).count > 1)
	{
		*((int*)((*((*itmPtr).previous))).val) += 0;
	}
#endif
	return(itmPtr);
}
void deleteItem(mlist* lis, item* itm)
{
	if ((*itm).next != NULL)
	{
		(*((*itm).next)).previous = (*itm).previous;
	}
	if ((*itm).previous != NULL)
	{
		(*((*itm).previous)).next = (*itm).next;
	}

	if (itm == (*lis).last)
	{
		(*lis).last = (*itm).previous;
	}
	if (itm == (*lis).first)
	{
		(*lis).first = (*itm).next;
	}

	//(*lis).avreg = ((*lis).avreg * (*lis).count - (*itm).val) / ((*lis).count - 1);
	(*lis).count -= 1;

	free(itm);
	//printf("item at %d was deleted\n", itm);
}
void deleteList(mlist* lis)
{
	struct item* current = (*lis).first;
	struct item* next;
	while ((*lis).count > 0)
	{
		next = (*current).next;
		free(current);
		current = next;
		(*lis).count -= 1;
	}
	free(lis);
}
void printItem(item* itm)
{
	printf("item at: %d has a value pointer to: %d\n", itm, (*itm).val);
}
void printlist(mlist* lis)
{
	printf("list at %d:\n", lis);
	struct item* printThis = (*lis).first;
	int i = 0;
	while (printThis != NULL)
	{
		printf("\t[%d] ", i);
		printItem(printThis);
		printThis = (*printThis).next;
		i++;
	}
	printf("end of list\n");
}
item* getItem(mlist* lis, int index)
{
	if (index < 0 || index >= (*lis).count)
	{
		return NULL;
	}
	struct item* itm = (*lis).first;
	for (int i = 0; i < index; ++i)
	{
		itm = (*itm).next;
	}
	return(itm);
}
item* firstPassingItem(mlist* lis, int(*testingFunc)(void*, void*), void* args) //used to find the first item that when passed to the function as an argument, recives a true value n return
{
	if (lis == NULL)
	{
		return(NULL);
	}
	item* thisItem = (*lis).first;
	while (thisItem != NULL && 0 == testingFunc((*thisItem).val, args))
	{
		thisItem = (*thisItem).next;
	}
#ifdef debugging
	if (thisItem != NULL)
	{
		*((int*)((*thisItem).val)) += 0;
	}
#endif
	return(thisItem);
}
void doForEach(mlist* lis, void(*func)(void*, void*), void* args)
{
	item* nextItem;
	if (lis == NULL)
	{
		return(NULL);
	}
	item* thisItem = (*lis).first;
	while (thisItem != NULL)
	{
		nextItem = (*thisItem).next;
		func((*thisItem).val, args);
		thisItem = nextItem;
	}
}
void* pop(mlist* lis)
{
	if ((*lis).count < 1)
	{
		return 0;
	}
	void* resaulte = (*(*lis).first).val;
	deleteItem(lis, (*lis).first);
	return(resaulte);
}

