#include "XA3.h"
#include<vector>
#include "GridPathingCell.h"

using namespace std;

GridPathingCell::GridPathingCell(uint16 xCord, uint16 yCord)
{
	x = (uint16)xCord;
	y = (uint16)yCord;
	xLast =  -1;
	yLast =  -1;
}

void GridPathingCell::setLast(uint16 xCord, uint16 yCord)
{
	xLast = xCord;
	yLast = yCord;
}

void GridPathingCell::setScore(uint16 newScore)
{
	score = newScore;
}

void GridPathingCell::reset()
{
	score = 32767;
	xLast = -1;
	yLast = -1;
}

uint16 GridPathingCell::getScore()
{
	return score;
}

TArray<int16> GridPathingCell::getLast()
{
	TArray<int16> last = TArray<int16>();
	last.Add(xLast);
	last.Add(yLast);
	return last;
}

uint16 GridPathingCell::getX()
{
	return x;
}

uint16 GridPathingCell::getY()
{
	return y;
}

GridPathingCell::GridPathingCell(const GridPathingCell & origin)
{
	x = origin.x;
	y = origin.y;
	score = origin.score;
	xLast = origin.xLast;
	yLast = origin.yLast;
}


GridPathingCell::~GridPathingCell()
{

}
