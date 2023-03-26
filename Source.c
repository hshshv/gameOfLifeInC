#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "worldFunctions.h"

//dirty shortcuts
#define until(x) while(!(x))
#define forever while(1)
#define foreachCellStart for(int x = 0; x < mapSize; ++x){for(int y = 0; y < mapSize; ++y){

/*--- GLOBAL VERIABLES ---*/
mlist* chunks;
mapType currentMap = typeMapA;
mapType currentNCMap = typeNcMapA;
mapType nextMap = typeMapB; 
mapType nextNCMap = typeNcMapB;

void printchunkLocOf(int x, int y)
{
	cellLoc cel = { x,y };
	chunkLoc cLoc = chunkLocOf(cel);
	printf("cunk location for cell (%d,%d) is [%d,%d]\n", x, y, cLoc.x, cLoc.y);
}
void printInChunkLocOf(int x, int y)
{
	cellLoc cel = { x,y };
	cellLoc inChun = inChunkLocationOf(cel);
	printf("in chunk location for (%d,%d) is (%d,%d)\n", x,y,inChun.x, inChun.y);
}
void testLocationsConversion()
{
	printf("testing conversions:\n");
	printchunkLocOf(0, 0); printf("answer above should be [%d,%d].\n", 0, 0);
	printInChunkLocOf(0, 0); printf("answer above should be (%d,%d).\n\n", 0, 0);

	printchunkLocOf(19, 20); printf("answer above should be [%d,%d].\n", 0, 1);
	printInChunkLocOf(19, 20); printf("answer above should be (%d,%d).\n\n", 19, 0);

	printchunkLocOf(-1, -1); printf("answer above should be [%d,%d].\n", -1, -1);
	printInChunkLocOf(-1, -1); printf("answer above should be (%d,%d).\n\n", 19, 19);

	printchunkLocOf(-20, -21); printf("answer above should be [%d,%d].\n", -1, -2);
	printInChunkLocOf(-20, -21); printf("answer above should be (%d,%d).\n\n", 0, 19);
}
int main(int argc, char* argv[])
{
//	testLocationsConversion();
	chunkLoc printFrom = { 0,0 };
	chunkLoc printTo = { -1,-1 };
	chunks = makeList();
	/*
	
	
	cellLoc loc = { 0, 0 };
	printchunkLocOf(0, 0);
	loc.x = 0; loc.y = 0;
	reviveAt(loc, currentMap);
	chunkLoc zero = { 0,0 };
	chunk* zeroChunk = getChunk(zero);
	printChunksRact(printFrom, printTo);


	loc.x = -1; loc.y = 0;
	reviveAt(loc, currentMap);
	printChunksRact(printFrom, printTo);

	loc.x = 0; loc.y = -1;
	reviveAt(loc, currentMap);
	printChunksRact(printFrom, printTo);

	loc.x = -1; loc.y = -1;
	reviveAt(loc, currentMap);
	printChunksRact(printFrom, printTo);
	//printChunkList(0);
	return(0);
	*/
	makeFirstMap();
	for (int i = 0; i < 50; ++i)
	{
		printf("cycel number %d:\n", i+1);
		printChunksRact(printFrom, printTo);
		doLifeCycle();
	}
	//printChunkList(0);
	return(0);
	
}