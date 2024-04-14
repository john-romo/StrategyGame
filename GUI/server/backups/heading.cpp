// headings.cpp


#include "heading.h"

Heading headings[8];

void create_headings(){
	EAST.id = 0;
	EAST.x = 1;
	EAST.y = 0;
	EAST.left = &NORTHEAST;
	EAST.right = &SOUTHEAST;
	EAST.reverse = &WEST;

	NORTHEAST.id = 1;
	NORTHEAST.x = 1;
	NORTHEAST.y = 1;
	NORTHEAST.left = &NORTH;
	NORTHEAST.right = &EAST;
	NORTHEAST.reverse = &SOUTHWEST;

	NORTH.id = 2;
	NORTH.x = 0;
	NORTH.y = 1;
	NORTH.left = &NORTHWEST;
	NORTH.right = &NORTHEAST;
	NORTH.reverse = &SOUTH;

	NORTHWEST.id = 3;
	NORTHWEST.x = -1;
	NORTHWEST.y = 1;
	NORTHWEST.left = &WEST;
	NORTHWEST.right = &NORTH;
	NORTHWEST.reverse = &SOUTHEAST;

	WEST.id = 4;
	WEST.x = -1;
	WEST.y = 0;
	WEST.left = &SOUTHWEST;
	WEST.right = &NORTHWEST;
	WEST.reverse = &EAST;

	SOUTHWEST.id = 5;
	SOUTHWEST.x = -1;
	SOUTHWEST.y = -1;
	SOUTHWEST.left = &SOUTH;
	SOUTHWEST.right = &WEST;
	SOUTHWEST.reverse = &NORTHEAST;

	SOUTH.id = 6;
	SOUTH.x = 0;
	SOUTH.y = -1;
	SOUTH.left = &SOUTHEAST;
	SOUTH.right = &SOUTHWEST;
	SOUTH.reverse = &NORTH;

	SOUTHEAST.id = 7;
	SOUTHEAST.x = 1;
	SOUTHEAST.y = -1;
	SOUTHEAST.left = &SOUTH;
	SOUTHEAST.right = &EAST;
	SOUTHEAST.reverse = &NORTHWEST;
	
}


Heading* START_HEADINGS[NUM_COLORS] = {&NORTH, &SOUTH};




