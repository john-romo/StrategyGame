// searchlight.h


#ifndef _SEARCHLIGHT_H
#define _SEARCHLIGHT_H

#include "defaults.h"
#include "piece.h"
#include "board.h"

class Searchlight : public Piece{
public:
	Searchlight(u8 _color);

	void reveal() override;
	void change_target(u8 _x, u8 _y) override;
};



#endif
