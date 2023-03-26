#pragma once

#include "mlist.h"

#define true 1
#define false 0
#define alive true
#define debugMode
#define chunkSize mapSize
#define mapSize 20

/*--- TYPES AND STRUCTS ---*/
#define cell int
#define mapType int

#define typeMapA 0
#define typeMapB 1
#define typeNcMapA 2
#define typeNcMapB 3

typedef cell MapArr[mapSize][mapSize];

//typedef cell(*Map)[mapSize];

struct chunkLocationStruct
{
	int x;
	int y;
};
typedef struct chunkLocationStruct chunkLoc;

struct cellLocStruct
{
	int x;
	int y;
};
typedef struct cellLocStruct cellLoc;

struct chunkStruct
{
	chunkLoc chunkLocation;
	int numOfAliveCells;
	MapArr* mapA;
	MapArr* mapB;
	MapArr* ncMapA;
	MapArr* ncMapB;
	int numOfCellsWithNCData;
};
typedef struct chunkStruct chunk;

