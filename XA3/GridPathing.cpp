#include "XA3.h"
#include "Grid.h"
#include "GridPathing.h"
#include "GridPathingCell.h"
#include <vector>

using namespace std;

GridPathing::GridPathing(TSharedPtr<Grid> g)
{
	grid = g;
	pathingArray = TArray<GridPathingCell>();
	for (int i = 0; i < (g->getWidth())*(g->getHeight()); i++) {
		uint16 xCord = i % (g->getWidth());
		uint16 yCord = i / (g->getWidth());
		pathingArray.Add(GridPathingCell(xCord, yCord));
	}
	finalPath = TArray<GridPathingCell>();
}

TArray<GridPathingCell> GridPathing::pathTo(uint16 xInit, uint16 yInit,uint16 xFinal,uint16 yFinal, int32 maxStep)
{
	reset();
	explorer(xInit, yInit, 0, maxStep);
	synthesis(xInit, yInit, xFinal,yFinal);
	return finalPath;
}

void GridPathing::explorer(uint16 x, uint16 y, uint16 score, int32 maxStep)
{
	pathingArray[calcIndex(x, y)].setScore(score);
	//================HORIZONTALS and VERTICALS FIRST=====================
	//x , y+1 direction
	if (isValidNextCell(x, y + 1, score + 2, maxStep - 4)) {
		pathingArray[calcIndex(x, y + 1)].setLast(x, y);
		explorer(x, y + 1, score + 2, maxStep - 4);
	}
	//x, y-1 direction
	if (isValidNextCell(x, y - 1, score + 2, maxStep - 4)) {
		pathingArray[calcIndex(x, y - 1)].setLast(x, y);
		explorer(x, y - 1, score + 2, maxStep - 4);
	}

	//x-1,y direction
	if (isValidNextCell(x - 1, y, score + 2, maxStep - 4)) {
		pathingArray[calcIndex(x - 1, y)].setLast(x, y);
		explorer(x - 1, y, score + 2, maxStep - 4);
	}
	//x+1,y direction
	if (isValidNextCell(x + 1, y, score + 2, maxStep - 4)) {
		pathingArray[calcIndex(x + 1, y)].setLast(x, y);
		explorer(x + 1, y, score + 2, maxStep - 4);
	}

	//================DIAGONALS=====================
	//Establishing corner conditions
	//block status of x,y+1
	bool block_Y_Plus = (!isValidNextCell(x, y + 1,0,10));
	//block status of x,y-1
	bool block_Y_Minus = (!isValidNextCell(x, y - 1, 0, 10));
	//block status of x+1,y
	bool block_X_Plus = (!isValidNextCell(x + 1, y, 0, 10));
	//block status of x,y+1
	bool block_X_Minus = (!isValidNextCell(x - 1, y, 0, 10));

	//x+1 , y+1 diagonal
	if (!block_Y_Plus && !block_X_Plus && isValidNextCell(x + 1, y + 1, score + 3, maxStep - 5)) {
		pathingArray[calcIndex(x + 1, y+1)].setLast(x, y);
		explorer(x + 1, y+1, score + 3, maxStep - 5);
	}

	//x+1 , y-1 diagonal
	if (!block_Y_Minus && !block_X_Plus && isValidNextCell(x + 1, y - 1, score + 3, maxStep - 5)) {
		pathingArray[calcIndex(x + 1, y - 1)].setLast(x, y);
		explorer(x + 1, y - 1, score + 3, maxStep - 5);
	}

	//x-1,y+1 diagonal
	if (!block_Y_Plus && !block_X_Minus && isValidNextCell(x - 1, y + 1, score + 3, maxStep - 5)) {
		pathingArray[calcIndex(x - 1, y + 1)].setLast(x, y);
		explorer(x - 1, y + 1, score + 3, maxStep - 5);
	}

	//x-1,y-1 diagonal
	if (!block_Y_Minus && !block_X_Minus && isValidNextCell(x - 1, y - 1, score + 3, maxStep - 5)) {
		pathingArray[calcIndex(x - 1, y - 1)].setLast(x, y);
		explorer(x - 1, y - 1, score + 3, maxStep - 5);
	}

	return;
}

uint16 GridPathing::calcIndex(uint16 x, uint16 y)
{
	return y*(grid->getWidth()) + x;
}

bool GridPathing::isValidNextCell(uint16 x, uint16 y, uint16 score, int32 maxStep)
{
	//enforcing boundary of grid
	if (x >= grid->getWidth() || y >= grid->getHeight() || x < 0 || y < 0) {
		return false;
	}

	//enforcing max Step
	if (maxStep <= 0) {
		return false;
	}

	//enforcing minimum score rule (do not visit cells with an already lower score/distance)
	if (pathingArray[calcIndex(x, y)].getScore() < score) {
		return false;
	}
	//if grid space is occupied, not valid next cell
	if (grid->isOccupied(x, y)) {
		return false;
	}
	//if grid space is a blocking space, not valid next cell
	if (grid->isBlocked(x, y)) {
		return false;
	}

	return true;


}

void GridPathing::synthesis(uint16 xInit, uint16 yInit, uint16 xFinal, uint16 yFinal)
{
	finalPath.Empty();
	if (pathingArray[calcIndex(xFinal,yFinal)].getLast()[0] == -1 && pathingArray[calcIndex(xFinal, yFinal)].getLast()[1] == -1)
	{
		return;
	}
	else {
		//we want to trace the path from finish to start here
		finalPath.Add(pathingArray[calcIndex(xFinal, yFinal)]);
		//tracker variables for current square
		uint16 xCurrent = xFinal;
		uint16 yCurrent = yFinal;
		while (!(finalPath.Last().getX() == xInit && finalPath.Last().getY() == yInit)) {
			xCurrent = finalPath.Last().getLast()[0];
			yCurrent = finalPath.Last().getLast()[1];
			finalPath.Push(pathingArray[calcIndex(xCurrent, yCurrent)]);
		}
		return;
	}
}



void GridPathing::reset()
{
	for (int i = 0; i < pathingArray.Num(); i++) {
		pathingArray[i].reset();
	}
}

bool GridPathing::isPathable(uint16 x, uint16 y)
{
	if (pathingArray[calcIndex(x, y)].getLast()[0] + pathingArray[calcIndex(x, y)].getLast()[1] != -2) {
		return true;
	}
	else {
		return false;
	}
}

GridPathing::~GridPathing()
{
}
