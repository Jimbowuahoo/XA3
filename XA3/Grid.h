#pragma once

/*! \class Grid
\brief Class for the Grid

Grid class that contains infromation on terrain, current occupation, and other properties
*/
class Grid 
{
public:
	//! Constructor taking in two unsigned integer values.
	/*!
	/param iH Height of the Grid
	/param iW Width of the grid
	Initiate the object with Height and Width values.
	*/
	Grid(uint16 iH, uint16 iW);

	//! Constructor taking in two unsigned integer values.
	/*!
	/param Tarr width and height of the grid in an array
	Initiate the object with Height and Width values.
	*/
	Grid(uint16 Tarr[]);

	//! Public member taking in two variables and returning an unsigned integer value.
	/*!
	/param ix x coordinate of the cell
	/param iy y coordinate of the cell
	Get the terrain value of the cell pertaining to the x and y coordinate.
	*/
	uint16 getTerrain(uint16 ix, uint16 iy);

	//! Public member taking in three variables.
	/*!
	/param ix x coordinate of the cell
	/param iy y coordinate of the cell
	/param iTerrain terrain value of the cell
	Set the terrain value of the cell pertaining to the x and y coordinate.
	*/
	void setTerrain(uint16 ix, uint16 iy, uint16 iTerrain);

	//! Public member taking in five variables.
	/*!
	/param ix x coordinate of the cell
	/param iy y coordinate of the cell
	/param iTerrain terrain value of the cell
	/param bBlocking boolean for if the cell is blocking movement
	/param bOccupied boolean for if the cell is occupied
	Set the properties of the grid cell
	*/
	void setProperties(uint16 ix, uint16 iy, uint16 iTerrain,bool bBlocking,bool bOccupied);

	//! Public member taking in two variables and returning a boolean value.
	/*!
	/param ix x coordinate of the cell
	/param iy y coordinate of the cell
	Get if the cell at the x and y coordinate is occupied.
	*/
	bool isOccupied(uint16 ix, uint16 iy);

	//! Public member taking in three variables.
	/*!
	/param ix x coordinate of the cell
	/param iy y coordinate of the cell
	/param bOccupied boolean for if the cell is occupied
	Set the bOccupied vallue at x and y.
	*/
	void setOccupied(uint16 ix, uint16 iy,bool bOccupied);

	//! Public member takeing in four variables.
	/*!
	/param iOldx x coordinate of the old cell
	/param iOldy y coordinate of the old cell
	/param iNewx x coordinate of the new cell
	/param iNewy y coordinate of the new cell
	Move something from one cell to another.
	*/
	void move(uint16 iOldX, uint16 iOldY, uint16 iNewX, uint16 iNewY);

	//! Public member taking in two variables and returning a boolean value.
	/*!
	/param ix x coordinate of the cell
	/param iy y coordinate of the cell
	Get if the cell at the x and y coordinate is blocked.
	*/
	bool isBlocked(uint16 ix, uint16 iy);
	//! Public member taking in three variables.
	/*!
	/param ix x coordinate of the cell
	/param iy y coordinate of the cell
	/param bBlocked boolean for if the cell is Blocked
	Set the bBlocked vallue at x and y.
	*/
	void setBlocked(uint16 ix, uint16 iy, bool bBlocked);

	//! Public member returning an unsigned int.
	/*!
	Get the Width of the grid.
	*/
	uint16 getWidth();

	//! Public member returning an unsigned int.
	/*!
	Get the Height of the grid.
	*/
	uint16 getHeight();
	~Grid();

private:
	//! A private variable.
	/*!
	Hold an array for the grid
	*/
	TArray<uint16> gridArray;

	uint16 iHeight, iWidth;
};