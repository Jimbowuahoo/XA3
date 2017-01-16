#pragma once

/*! \class GridPathingCell
\brief Class for Cells in the grid, required for pathin

A class that contains perameters of the grid pathing cell, an object that represents individual cells
in a grid. Used in the GridPathing class
*/
class GridPathingCell 
{
public:
	//! A public variable.
	/*!
	X coordinate of the cell.
	*/
	uint16 x;
	//! A public variable.
	/*!
	Y coordinate of the cell.
	*/
	uint16 y;
	//! A public variable.
	/*!
	Cost of reaching this cell from the origin, with diagonals worth more than horizontals and verticals
	*/
	uint16 score;

	//! A public variable.
	/*!
	X coordinate of the last cell to reach this one from the origin with the lowest score.
	*/
	int16 xLast;
	//! A public variable.
	/*!
	Y coordinate of the last cell to reach this one from the origin with the lowest score.
	*/
	int16 yLast;

	//! Constructor taking in two unsigned integer values.
	/*!
	/param xCord x coordinate of the cell
	/param yCord y coordinate of the cell
	Initiate the object with x and y coordinates.
	*/
	GridPathingCell(uint16 xCord,uint16 yCord);

	//! A public member that takes in two variables.
	/*!
	/param xCord x value to set xLast to
	/param yCord y value to set yLast to
	Set the last X Y coordinates of the cell.
	*/
	void setLast(uint16 xCord, uint16 yCord);

	//! A public member that takes in a variables.
	/*!
	/param score score to be set to
	Set score of the cell.
	*/
	void setScore(uint16 score);

	//! A public member.
	/*!
	Reset the cell.
	*/
	void reset();

	//! A public member returning an unsigned integer value.
	/*!
	Get the score of the cell.
	*/
	uint16 getScore();

	//! A public member returning an integer array.
	/*!
	Get the xLast and yLast values as an array.
	*/
	TArray<int16> getLast();

	//! A public member returning an unsigned integer value.
	/*!
	Get X.
	*/
	uint16 getX();
	//! A public member returning an unsigned integer value.
	/*!
	Get Y.
	*/
	uint16 getY();

	//! Constructor taking in a GridPathingCell.
	/*!
	/param origin the origin cell
	Grid Pathing constructor with a Grid Pathing object.
	*/
	GridPathingCell(const GridPathingCell &origin);

	~GridPathingCell();
	
};

