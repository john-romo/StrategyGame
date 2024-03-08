// heading.h


#ifndef _HEADING_H
#define _HEADING_H

#include "default.h"

class Heading{
public:
	i8 x;
	i8 y;

	Heading* left;
	Heading* right;
	Heading* reverse;

};

extern Heading headings[8];
extern Heading* START_HEADINGS[NUM_PLAYERS];

void create_headings();



#endif
