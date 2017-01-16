#pragma once
class GridPathingCell;

/*! \class GridPathing
\brief Class for Pathing Units on the Grid

A class that uses the GridPathingCell class to determine the closest path from one point on the grid
to another. 
*/
class GridPathing
{
public:
	/* Constructor
	* Initializes the Grid Pathing object
	*/
	GridPathing(TSharedPtr<Grid> g);
	//! A public array variable.
	/*!
	Array that contains cells that are in the final path between the two points.
	*/
	TArray<GridPathingCell> finalPath;

	//! A public member taking in five variables returning a GridPathingCell Array.
	/*!
	/param xInit x value of the starting cell
	/param yInit y value of the starting cell
	/param xFinal x value of the ending cell
	/param yFinal y value of the ending cell
	/param maxStep the maximum number of steps any unit can take, to limit the number of cells the pathing algorithm will search for
	Uses recursive methods to return the final path.
	*/
	TArray<GridPathingCell> pathTo(uint16 xInit, uint16 yInit, uint16 xFinal, uint16 yFinal, int32 maxStep);

	//! A public member taking in four variables.
	/*!
	/param x x value of the cell
	/param y y value of the cell
	/param score score of the path
	/param maxStep the maximum number of steps any unit can take, to limit the number of cells the pathing algorithm will search for
	A method to recursively visits each cell of the grid in a certain path to score it and point the cells to the previous cell in the optimal path.
	*/
	void explorer(uint16 x, uint16 y,uint16 score,int32 maxStep);

	//! A public member taking in two variables and returning a variable.
	/*!
	/param x x value of the cell
	/param y y value of the cell
	A method to calculate the index of the cell.
	*/
	uint16 calcIndex(uint16 x, uint16 y);

	//! A public member taking in four variables and returning a boolean value.
	/*!
	/param x x value of the cell
	/param y y value of the cell
	/param score score of the path
	/param maxStep the maximum number of steps any unit can take, to limit the number of cells the pathing algorithm will search for
	A method to deterimine if the next cell is a valid one.
	*/
	bool isValidNextCell(uint16 x, uint16 y, uint16 score, int32 maxStep);

	//! A public member taking in four variables.
	/*!
	/param xInit x value of the starting cell
	/param yInit y value of the starting cell
	/param xFinal x value of the ending cell
	/param yFinal y value of the ending cell
	A method that takes the now filled matrix of scores and "connected nodes" (cell objects which has a valid lastX and lastY) and traces out a path from the origin to a certain destination cell.
	*/
	void synthesis(uint16 xInit, uint16 yInit, uint16 xFinal, uint16 yFinal);

	//! A public member.
	/*!
	A method to reset the pathing array
	*/
	void reset();

	//! A public member taking in two variables and returning a boolean value.
	/*!
	/param x x value of the cell
	/param y y value of the cell
	A method to deterimine whether a path can be calculated based on the x and y location of the cell.
	*/
	bool isPathable(uint16 x, uint16 y);

	//! A public Pointer Array.
	/*!
	The grid that pathing is done on.
	*/
	TSharedPtr<Grid> grid;

	//! A public Pointer Array.
	/*!
	The array of cells used to path.
	*/
	TArray<GridPathingCell> pathingArray;
	~GridPathing();
private:
	
};

