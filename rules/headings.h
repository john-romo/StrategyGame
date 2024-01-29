// headings.h


#ifndef _HEADINGS_H
#define _HEADINGS_H

#include "defaults.h"

class Heading{
public:
	i8 x;
	i8 y;

	Heading* left;
	Heading* right;
	Heading* reverse;

};

extern Heading headings[8];

void create_headings();



#endif
