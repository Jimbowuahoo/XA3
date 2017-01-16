#include "XA3.h"
#include "Grid.h"

Grid::Grid(uint16 im, uint16 in)
{
	//gridPtr = (uint16 *) FMemory::Malloc(im*in * sizeof(uint16));
	gridArray = TArray<uint16>();
	for (int i = 0; i < im*in; i++) {
		gridArray.Add((uint16)31);
	}
	iHeight = im;
	iWidth = in;
}

Grid::Grid(uint16 TArr[])
{
	uint16 im = TArr[0];
	uint16 in = TArr[1];
	/*
	gridPtr = (uint16 *)FMemory::Malloc(im*in * sizeof(uint16));
	for (int i = 2; i < im*in + 2; i++) {
		gridPtr[i - 2] = TArr[i];
	}
	*/
	gridArray = TArray<uint16>();
	for (int i = 2; i < im*in + 2; i++) {
		gridArray.Add(TArr[i]);
	}
	iHeight = in;
	iWidth = im;
}

uint16 Grid::getTerrain(uint16 ix, uint16 iy)
{
	return abs(gridArray[iy*iWidth +ix]%100);
}

void Grid::setTerrain(uint16 ix, uint16 iy, uint16 iTerrain)
{
	gridArray[iy*iWidth + ix] = (gridArray[iy*iWidth + ix] / 1000) * 1000 + iTerrain;
}

void Grid::setProperties(uint16 ix, uint16 iy, uint16 iTerrain, bool bBlocking, bool bOccupied)
{
	gridArray[iy*iWidth + ix] = iTerrain;
	if (bBlocking) {
		gridArray[iy*iWidth + ix] += 1000;
	}
	if (bOccupied) {
		gridArray[iy*iWidth + ix] += 100;
	}
}

bool Grid::isOccupied(uint16 ix, uint16 iy)
{
	if (gridArray[iy*iWidth + ix]%1000 > 100) {
		return true;
	}
	return false;

}

void Grid::setOccupied(uint16 ix, uint16 iy, bool bOccupied)
{
	if (bOccupied) {
		gridArray[iy*iWidth + ix] = gridArray[iy*iWidth + ix]/1000 *1000+100+ gridArray[iy*iWidth + ix]%100;
	}
	else {
		gridArray[iy*iWidth + ix] = gridArray[iy*iWidth + ix] / 1000 * 1000  + gridArray[iy*iWidth + ix] % 100;
	}
}

void Grid::move(uint16 iOldX, uint16 iOldY, uint16 iNewX, uint16 iNewY)
{
	setOccupied(iOldX,iOldY,false);
	setOccupied(iNewX, iNewY, true);
	
}

bool Grid::isBlocked(uint16 ix, uint16 iy)
{
	if (abs(gridArray[iy*iWidth + ix]) >= 1000 ) {
		return true;
	}
	return false;
}

void Grid::setBlocked(uint16 ix, uint16 iy, bool bBlocked)
{
	gridArray[iy*iWidth + ix] = (gridArray[iy*iWidth + ix] % 1000);
	if (bBlocked) {
		gridArray[iy*iWidth + ix] += 1000;
	}
}

uint16 Grid::getWidth()
{
	return iWidth;
}

uint16 Grid::getHeight()
{
	return iHeight;
}

Grid::~Grid()
{

	//FMemory::Free(gridPtr);
}
