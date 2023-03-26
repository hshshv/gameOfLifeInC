#include "worldFunctions.h"
#include <malloc.h>
#include <math.h>

#define debugging

extern mapType nextMap;
extern mapType nextNCMap;
extern mapType currentMap;
extern mapType currentNCMap;
extern mlist* chunks;

/*--- FUNCTIONS DEFENISIONS ---*/

//lambdas
int compChunkPtrToChunkLoc(void* ptrToChunk, void* ptrToChunkLoc)
{
	chunk* chun = (chunk*)ptrToChunk;
	chunkLoc* cLoc = (chunkLoc*)ptrToChunkLoc;
	if (((*chun).chunkLocation.x == (*cLoc).x && (*chun).chunkLocation.y == (*cLoc).y))
	{
		return(1);
	}
	else
	{
		return(0);
	}
}
void initializeChunk_Lambda(void* chunkPtr, void* args_unused)
{
	initializeMap(getMapInChunk((chunk*)chunkPtr, nextMap));
	initializeMap(getMapInChunk((chunk*)chunkPtr, nextNCMap));
	(*((chunk*)chunkPtr)).numOfAliveCells = 0;
	(*((chunk*)chunkPtr)).numOfCellsWithNCData = 0;
}
void doChunkLifeCycle_Lambda(void* chunkPtr, void* args_unused)
{
	//printf("doChunkLifeCycle_Lambda - chunk at %d,%d\n", (*((chunk*)chunkPtr)).chunkLocation.x, (*((chunk*)chunkPtr)).chunkLocation.y);
	doChunkLifeCycle((chunk*)chunkPtr);
}
void checkIfChunkIsAlive_Lambda(void* chunkPtr, void* args_unused)
{
	chunk* thisChunk = (chunk*)chunkPtr;
	if ((*thisChunk).numOfCellsWithNCData == 0)//we should keep chunks that dont have living cells if they have NC data
	{
//		deleteItem(chunks, firstPassingItem(chunks, compChunkPtrToChunkLoc, &((*thisChunk).chunkLocation)));
	//	deleteChunk(thisChunk);
	}
}
void printDualMapAndChunkLocation_Lambda(void* chunkPtr, void* args_unused)
{
	//prints both map and ncMap of the current map in the selected chunk
	chunkLoc cLoc = (*(chunk*)chunkPtr).chunkLocation;
	printf("chunk location: %dx,%dy\n", cLoc.x, cLoc.y);
	printDual((chunk*)chunkPtr);
}
void printMap_Lambda(void* chunkPtr, void* arg_printNc)
{
	chunk* thisChunk = (chunk*)chunkPtr;
	printChunkData(thisChunk);
	if (*(int*)arg_printNc == 2)
	{
		printDual(thisChunk);
	}
	else if (*(int*)arg_printNc == 1)
	{
		printMap(getMapInChunk(thisChunk, currentNCMap), true);
	}
	else
	{
		printCells(getMapInChunk(thisChunk, currentMap));
	}
}
void printChunk_lambda(void* chunkPtr, void* args_unused)
{
	printChunkData((chunk*)chunkPtr);
}

//aritmatics and conversions
chunkLoc chunkLocOf(cellLoc cellAbsLoc)
{
	chunkLoc chun = { 0, 0 };
	if (cellAbsLoc.x >= 0)
	{
		chun.x = cellAbsLoc.x / chunkSize;
	}
	else
	{
		chun.x = ((cellAbsLoc.x + 1) / chunkSize) - 1;
	}
	if (cellAbsLoc.y >= 0)
	{
		chun.y = cellAbsLoc.y / chunkSize;
	}
	else
	{
		chun.y = ((cellAbsLoc.y + 1) / chunkSize) - 1;
	}
	return(chun);
}
cellLoc inChunkLocationOf(cellLoc absLoc)
{
	chunkLoc cLoc = chunkLocOf(absLoc);
	cellLoc corner = { chunkSize * cLoc.x, chunkSize * cLoc.y };
	cellLoc answer = { absLoc.x - corner.x, absLoc.y - corner.y };
	return(answer);
}
MapArr* getMapInChunk(chunk* chunkPtr, mapType map)
{
#ifdef debugging
	if (chunkPtr == NULL)
	{
		printf("recived NULL\n");
		while (1) {}
	}
#endif

	if (map == typeMapA)
	{
		return((*chunkPtr).mapA);
	}
	if (map == typeMapB)
	{
		return((*chunkPtr).mapB);
	}
	if (map == typeNcMapA)
	{
		return((*chunkPtr).ncMapA);
	}
	if (map == typeNcMapB)
	{
		return((*chunkPtr).ncMapB);
	}
	return(NULL);
}
MapArr* getFirstMap()
{
	item* PtrToChunkItem = (*chunks).first;
	chunk* chunkPtr = (*PtrToChunkItem).val;
	MapArr* ans = getMapInChunk(chunkPtr, currentMap);

#ifdef debugging
	if (ans == NULL)
	{
		printf("returning NULL\n");
		while (1) {}
	}
#endif

	return(ans);
}

//world access
//temp:
chunk* getChunk(chunkLoc cLoc)
{
	static chunk* lastChunk = NULL;
	//caching - chunking is the requested chunk is the same chunk as the last reqested chunk, so we wont have to look up for it again. if not - serching for it.
	chunk* thisChunkPtr = NULL; //maby get rid of it
	if (lastChunk != NULL)
	{
		if (compChunkPtrToChunkLoc(lastChunk, &cLoc) == true) // the requested chunk is the same as the former
		{
			thisChunkPtr = lastChunk;
		}
	}
	if (thisChunkPtr == NULL) // request of a chunk that isnt the last, or there is no last chunk: serch for the new chunk
	{
		item * thisItem = firstPassingItem(chunks, compChunkPtrToChunkLoc, &cLoc);
		if (thisItem != NULL)
		{
			thisChunkPtr = (chunk*)((*thisItem).val);
		}
	}
	if (thisChunkPtr == NULL) // the chunk doesnt exist
	{
		thisChunkPtr = makeChunk(cLoc);
		item* lit = addItem(chunks, (void*)thisChunkPtr);
		//printf("added chunk. item properties are:\n\titem is at %d\n\tnext is at %d\n\tprevius is at %d\n", lit, (*lit).next, (*lit).previous);
	}
	lastChunk = thisChunkPtr;
	
#ifdef debugging
	if (thisChunkPtr == NULL)
	{
		printf("returning NULL\n");
		while (1) {}
	}
#endif
	return(thisChunkPtr);
}
cell* getCellWC(cellLoc absCellLoc, chunk* chunkPtr, mapType mapTy)
{
	//set in-chunk location
	cellLoc ICLoc = inChunkLocationOf(absCellLoc);

	//return apropriate cell according to the requested map
	MapArr* mapPtr = getMapInChunk(chunkPtr, mapTy);
	cell* resaulte = &((*mapPtr)[ICLoc.x][ICLoc.y]);

#ifdef debugging
		if (resaulte == NULL)
		{
			printf("returning NULL\n");
			while (1) {}
		}
#endif
		return(resaulte);
}
cell* getCell(const cellLoc absCellLoc, mapType mapTy)
{
	chunkLoc cLoc = chunkLocOf(absCellLoc);
	chunk* thisChun = getChunk(cLoc);
	cell* thisCell = getCellWC(absCellLoc, thisChun, mapTy);
	return(thisCell);
}
void reviveAt(cellLoc absLocation, mapType map)
{
	/*
	steps:
		retrive the relevent chunk
		set the relevent cell to 'alive'
		increament the living cells count on the chunk
		inceas the negibor cout of the cells negibors
	*/

	chunk* thisChunk = getChunk(chunkLocOf(absLocation));
	//printf("\t[function] reviveAt: %d,%d\n", absLocation.x, absLocation.y);
	(*getCellWC(absLocation, thisChunk, map)) = alive;
	(*thisChunk).numOfAliveCells += 1;
	mapType thisNcMap;
	if (map == typeMapA)
	{
		thisNcMap = typeNcMapA;
	}
	else
	{
		thisNcMap = typeNcMapB;
	}
	increasNegibors(absLocation, thisNcMap);
}
void increasNCForNegibor(cellLoc negiborLoc, mapType map)
{
	static chunk* thisChunk = NULL;
	thisChunk = getChunk(chunkLocOf(negiborLoc));
	++(*getCellWC(negiborLoc, thisChunk, map));
	++(*thisChunk).numOfCellsWithNCData;
}
void increasNegibors(cellLoc loc, mapType map)
{
	cellLoc negibor = { loc.x - 1, loc.y - 1 };
	increasNCForNegibor(negibor, map);
	++negibor.x;
	increasNCForNegibor(negibor, map);
	++negibor.x;
	
	increasNCForNegibor(negibor, map);

	negibor.x -= 2; ++negibor.y;
	if (negibor.y == 17 && negibor.x == 18)
	{
		negibor.x += 0;// for the breakpoint

	}
	increasNCForNegibor(negibor, map);
	++negibor.x;
	//increasNCForNegibor(negibor, map); a cell isnt a negibor of itself
	++negibor.x;
	increasNCForNegibor(negibor, map);

	negibor.x -= 2; ++negibor.y;
	increasNCForNegibor(negibor, map);
	++negibor.x;
	increasNCForNegibor(negibor, map);
	++negibor.x;
	increasNCForNegibor(negibor, map);

	/*
	if (isInside(loc.x + 1, loc.y + 1)) { ++ncMap[loc.x + 1][loc.y + 1]; }
	if (isInside(loc.x + 1, loc.y + 0)) { ++ncMap[loc.x + 1][loc.y + 0]; }
	if (isInside(loc.x + 1, loc.y - 1)) { ++ncMap[loc.x + 1][loc.y - 1]; }
	if (isInside(loc.x + 0, loc.y + 1)) { ++ncMap[loc.x + 0][loc.y + 1]; }
	//if (isInside(loc.x + 0, loc.y + 0)) { ++ncMap[loc.x + 0][loc.y + 0]; } // a cell isnt itselfs negibor
	if (isInside(loc.x + 0, loc.y - 1)) { ++ncMap[loc.x + 0][loc.y - 1]; }
	if (isInside(loc.x - 1, loc.y + 1)) { ++ncMap[loc.x - 1][loc.y + 1]; }
	if (isInside(loc.x - 1, loc.y + 0)) { ++ncMap[loc.x - 1][loc.y + 0]; }
	if (isInside(loc.x - 1, loc.y - 1)) { ++ncMap[loc.x - 1][loc.y - 1]; }
	*/
}
int chunkExist(chunkLoc cLoc)
{
	item* chunkItem = firstPassingItem(chunks, compChunkPtrToChunkLoc, (void*)(&cLoc));
	if (chunkItem == NULL)
	{
		return(0);
	}
	else
	{
		return(1);
	}
}

//basic data manegment
void initializeMap(MapArr* map)
{
	for (int x = 0; x < mapSize; ++x)
	{
		for (int y = 0; y < mapSize; ++y)
		{
			(*map)[x][y] = 0;
		}
	}
}

//main program actions
void makeFirstMap()
{
	MapArr initialMap =
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
		{0,0,1,0,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
		{0,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	//add living cells to thisMap using reviveAt()
	for (cellLoc loc = { 0,0 }; loc.x < mapSize; ++loc.x)
	{
		for (loc.y = 0; loc.y < mapSize; ++loc.y)
		{
			if (initialMap[loc.x][loc.y] > 0)
			{
				reviveAt(loc, currentMap);
			}
		}
	}
}
void doLifeCycle()
{
	/*
	steps:
	starting with currentMap and currentNCmap full and completed.

	for each chunk in the chunk list: initialize chunk:
		initialize nextMap
		initialize nextNcMap
		set number of living cells to 0
	for each chunk in the chunk list: doChunkLifeCycle
	for each chunk in the chunk list: check if the chunk is dead, if so: delete the chunk (complecated I assum)

	replace nextMap <> currentMap, and nextNCMap <> currentNCMAp
	*/

	doForEach(chunks, initializeChunk_Lambda, NULL);
	doForEach(chunks, doChunkLifeCycle_Lambda, NULL);
	doForEach(chunks, checkIfChunkIsAlive_Lambda, NULL);

	mapType tempMap = currentMap;
	currentMap = nextMap;
	nextMap = tempMap;
	tempMap = currentNCMap;
	currentNCMap = nextNCMap;
	nextNCMap = tempMap;
}
void doChunkLifeCycle(chunk* thisChunk)
{

	/*
	steps:
	starting with a thisMap with updated count table, while next map and nextNC are initialized or with some value already (currently under edit)

	for each cell in current map - cheak if should be alive in the next one
		if yes -> revive it in the next map using reviveAt so the nextNCmap will also be updated
	*/

	for
		(
			cellLoc absLocation = { chunkSize * (*thisChunk).chunkLocation.x, chunkSize * (*thisChunk).chunkLocation.y }; //set starting position to bottom-left corner of the chunk
			absLocation.x < chunkSize* ((*thisChunk).chunkLocation.x + 1); //  ending position is the starting position + chunksize
			++absLocation.x
			)
	{
		for
			(
				absLocation.y = chunkSize * (*thisChunk).chunkLocation.y;
				absLocation.y < chunkSize* ((*thisChunk).chunkLocation.y + 1);
				++absLocation.y
				)
		{
			if
				(
					(*getCell(absLocation, currentNCMap) == 2) && (*getCell(absLocation, currentMap) == alive)
					|| (*getCell(absLocation, currentNCMap) == 3)
					)
			{
				reviveAt(absLocation, nextMap);
			}
		}
	}
}

//display
void printMap(const MapArr* map, int printNC)
{
	printf("\n");
	for (int x = 0; x < mapSize; ++x)
	{
		for (int y = 0; y < mapSize; ++y)
		{
			if (printNC == true)
			{
				printf("%d ", (*map)[x][y]);
			}
			else
			{
				if ((*map)[x][y] == alive)
				{
					printf("# ");
				}
				else
				{
					printf(". ");
				}
			}
		}
		printf("\n");
	}
	printf("---------------\n");
}
void printCells(const MapArr* map)
{
	printMap(map, false);
}
void printNcMap(const MapArr* map)
{
	printMap(map, true);
}
void printDual(const chunk* chunkPtr)
{
	printf("chunk Location: %dx,%dy\n", (*chunkPtr).chunkLocation.x, (*chunkPtr).chunkLocation.y);;
	MapArr* map = getMapInChunk(chunkPtr, currentMap);
	MapArr* ncMap = getMapInChunk(chunkPtr, currentNCMap);
	for (cellLoc loc = { 0,0 }; loc.x < mapSize; ++loc.x)
	{
		for (loc.y = 0; loc.y < mapSize; ++loc.y)
		{
			if ((*map)[loc.x][loc.y] == alive)
			{
				printf("# ");
			}
			else
			{
				printf(". ");
			}
		}
		printf("\t");
		for (loc.y = 0; loc.y < mapSize; ++loc.y)
		{
			if ((*map)[loc.x][loc.y] == alive)
			{
				printf(".%d", (*ncMap)[loc.x][loc.y]);;
			}
			else
			{
				printf(" %d", (*ncMap)[loc.x][loc.y]);
			}
			
		}
		printf("\n");
	}
	printf("---------------\n");
}
void printChunksRact(chunkLoc from, chunkLoc to)
{
	printf("printing selected chunks:\n");
	chunkLoc lowLoc = { (int)fmin((double)from.x, (double)to.x), (int)fmin((double)from.y, (double)to.y) };
	chunkLoc highLoc = { (int)fmax((double)from.x, (double)to.x), (int)fmax((double)from.y, (double)to.y) };
	//printf("chunk Range is : %d,%d - %d,%d\n", lowLoc.x, lowLoc.y, highLoc.x, highLoc.y);
	int minX = lowLoc.x * chunkSize;
	int minY = lowLoc.y * chunkSize;
	int maxX = (highLoc.x + 1) * chunkSize - 1;
	int maxY = (highLoc.y + 1) * chunkSize - 1;
	//printf("cell Range is : %d,%d - %d,%d\n", minX, minY, maxX, maxY);
	
	for (cellLoc loc = { minX, maxY - 1 }; loc.x < maxX; ++loc.x)
	{
		for (loc.y = maxY - 1; loc.y >= minY; --loc.y)
		{
			if (chunkExist(chunkLocOf(loc)) == 1)
			{
				if (*getCell(loc, currentMap) == alive)
				{
					printf("# ");
					continue;
				}
			}
			printf(". ");
		}
		printf("\n");
	}
	printf("---------------\n");
}
void printChunkList(int printNC)
{
	doForEach(chunks, printMap_Lambda, (void*)(&printNC));
}
void printChunkData(chunk* thisChunk)
{
	printf("chunk at %d,%d with %d living cells and %d cells with ncData\n", (*thisChunk).chunkLocation.x, (*thisChunk).chunkLocation.y, (*thisChunk).numOfAliveCells, (*thisChunk).numOfCellsWithNCData);
}

//constructors / destructors
chunk* makeChunk(chunkLoc chunkLoc)
{
	printf("making new chunk at [%d,%d]\n", chunkLoc.x, chunkLoc.y);
	chunk tempChunk;
	tempChunk.chunkLocation = chunkLoc;
	tempChunk.numOfAliveCells = 0;
	tempChunk.mapA = makeMap();
	tempChunk.mapB = makeMap();
	tempChunk.ncMapA = makeMap();
	tempChunk.ncMapB = makeMap();
	tempChunk.numOfCellsWithNCData = 0;

	/* =
	{
		chunkLoc, 0, makeMap(), makeMap(), makeMap(), makeMap()
	};*/
	chunk* thisChunk = (chunk*)malloc(sizeof(chunk));
	(*thisChunk) = tempChunk;

#ifdef debugging
	if (thisChunk == NULL)
	{
		printf("returning NULL\n");
		while (1) {}
	}
#endif

	return(thisChunk);
}
void deleteChunk(chunk* chunkPtr)
{
	/*
	struct chunkStruct
	{
		chunkLoc chunkLocation;
		int numOfAliveCells;
		Map mapA;
		Map mapB;
		Map ncMapA;
		Map ncMapB;
	};
	*/
	free((*chunkPtr).mapA);
	free((*chunkPtr).mapB);
	free((*chunkPtr).ncMapA);
	free((*chunkPtr).ncMapB);
	free(chunkPtr);
	printf("deleted chunk at %d", chunkPtr);
}
MapArr* makeMap()
{
	MapArr* map = (MapArr*)malloc(sizeof(MapArr));
	//printf("allocated %d byts of memory\n", sizeof(MapArr));
	initializeMap(map);
	return(map);
}

//unneccery?
/*
void deleteMap(Map map)
{
	for (int x = 0; x < mapSize; ++x)
	{
		free(map[x]);
	}
}*/
/*int isInside(int x, int y)
{
	if (x > mapSize - 1 || x < 0 || y > mapSize - 1 || y < 0)
	{
		return(false);
	}
	return(true);
}*/
/*void setCell(cellLoc location, cell status, cell(*map)[mapSize])
{
	if (map == NULL)
	{
#ifdef debugMode
		printf("[function] [setCell] map pointer was NULL\n");
#endif
		return(0);
	}
	if (!isInside(x, y))
	{
		return;
	}
	map[x][y] = status;
}*/
/*
void doNegiborsCount(cell(*map)[mapSize], cell(*ncmap)[mapSize])
{
	initializeMap(ncmap);
	for (int x = 0; x < mapSize; ++x)
	{
		for (int y = 0; y < mapSize; ++y)
		{
			if (isAliveAt(x, y, thisMap))
			{
				increasNegibors(x, y, ncmap);
			}
		}
	}
}
*/ //
/*int isAliveAt(cellLoc loc, cell(*map)[mapSize])
{/*
	if (map == NULL)
	{
#ifdef debugMode
		printf("[function] [isAliveAt] map pointer was NULL\n");
#endif
		return(0);
	}
	if (!isInside(x, y))
	{
		return(0);
	}
	return(map[x][y]);
}
*/
/*int chunckExist(chunkLoc chunkLoc)
{
	struct item *itmPtr = (*chunks).first;
	chunk* chPtr;
	while (itmPtr != NULL)
	{
		chPtr = (chunk*)((*itmPtr).val);
		if (chPtr == NULL)
		{
#ifdef debugMode
			printf("[function] [chunkExist] chunk pointer within an item was NULL");
			//it is possible to create chunk and fix, but if we got here we are in a problem anyway so it doesnt matter so much
#endif
		}
		if ((*chPtr).chunkLocation.x == chunkLoc.x && (*chPtr).chunkLocation.y == chunkLoc.y)
		{
			return(true);
		}
		itmPtr = (*itmPtr).next;
	}
	return(false);
}*/
/*void ensureChunck(chunkLoc chunkLoc)
{
	if (chunckExist(chunkLoc) == false)
	{
		addItem(chunks, (void*)makeChunk(chunkLoc));
	}
}*/