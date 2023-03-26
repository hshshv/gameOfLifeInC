#pragma once

#include "specialTypes.h"

/*--- FUNCTION DECLERATION ---*/

//aritmatics and conversions
chunkLoc chunkLocOf(cellLoc loc);
cellLoc inChunkLocationOf(cellLoc absLoc);
MapArr* getMapInChunk(chunk* chunkPtr, mapType map);
MapArr* getFirstMap();

//world access
chunk* getChunk(chunkLoc cLoc);
cell* getCellWC(cellLoc absCellLoc, chunk* chunkPtr, mapType map);
cell* getCell(const cellLoc absCellLoc, mapType mapTy);
void reviveAt(cellLoc location, mapType map);
void increasNegibors(cellLoc location, mapType map);
int chunkExist(chunkLoc cLoc);

//basic data maneging
void initializeMap(MapArr* map);

//deispaly
void printMap(const MapArr* map, int printNC);
void printCells(const MapArr* map);
void printNcMap(const MapArr* map);
void printDual(const chunk* chunkPtr);
void printChunksRact(chunkLoc from, chunkLoc to);
void printChunkList(int printNC);
void printChunkData(chunk* thisChunk);

//main program actions
void makeFirstMap();
void doLifeCycle();
void doChunkLifeCycle(chunk* thisChunk);

//constructors / destructors
chunk* makeChunk(chunkLoc chunkLoc);
void deleteChunk(chunk* chunkPtr);
MapArr* makeMap();
