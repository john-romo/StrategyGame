// headings.cpp


#include "headings.h"

Heading headings[8];

void create_headings(){
	NORTH.x = 0;
	NORTH.y = 1;
	NORTH.left = &NORTHWEST;
	NORTH.right = &NORTHEAST;
	NORTH.reverse = &SOUTH;

	NORTHWEST.x = -1;
	NORTHWEST.y = 1;
	NORTHWEST.left = &WEST;
	NORTHWEST.right = &NORTH;
	NORTHWEST.reverse = &SOUTHEAST;

	WEST.x = -1;
	WEST.y = 0;
	WEST.left = &SOUTHWEST;
	WEST.right = &NORTHWEST;
	WEST.reverse = &EAST;
	SOUTHWEST.x = -1;
	SOUTHWEST.y = -1;
	SOUTHWEST.left = &SOUTH;
	SOUTHWEST.right = &WEST;
	SOUTHWEST.reverse = &NORTHEAST;

	SOUTH.x = 0;
	SOUTH.y = -1;
	SOUTH.left = &SOUTHEAST;
	SOUTH.right = &SOUTHWEST;
	SOUTH.reverse = &NORTH;

	SOUTHEAST.x = 1;
	SOUTHEAST.y = -1;
	SOUTHEAST.left = &SOUTH;
	SOUTHEAST.right = &EAST;
	SOUTHEAST.reverse = &NORTHWEST;

	EAST.x = 1;
	EAST.y = 0;
	EAST.left = &NORTHEAST;
	EAST.right = &SOUTHEAST;
	EAST.reverse = &WEST;

	NORTHEAST.x = 1;
	NORTHEAST.y = 1;
	NORTHEAST.left = &NORTH;
	NORTHEAST.right = &EAST;
	NORTHEAST.reverse = &SOUTHWEST;
}
