// heading.h


#ifndef _HEADING_H
#define _HEADING_H

#include "../default.h"

class Heading{
public:
	int id;

	int x;
	int y;

	Heading* left;
	Heading* right;
	Heading* reverse;

};

extern Heading headings[8];
extern Heading* START_HEADINGS[NUM_COLORS];

void create_headings();



#endif
