// guard.h


#ifndef _GAURD_H
#define _GAURD_H

#include "defaults.h"
#include "piece.h"
#include "board.h"

class Guard : public Piece{
public:
	Guard(u8 _color);

	void reveal() override;
};

#endif
