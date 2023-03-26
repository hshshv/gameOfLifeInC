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
	chunkLoc printFrom = { 1,1 };
	chunkLoc printTo = { -2,-2 };
	chunks = makeList();
	makeFirstMap();
	for (int i = 0; i < 50; ++i)
	{
		printf("cycel number %d:\n", i+1);
		printChunksRact(printFrom, printTo);
		doLifeCycle();
	}
	printChunkList(0);
	return(0);
	
}