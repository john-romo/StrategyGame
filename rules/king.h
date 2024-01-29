// king.h


#ifndef _KING_H
#define _KING_H

#include "defaults.h"
#include "piece.h"
#include "board.h"

class King : public Piece{
public:
	King(u8 _color);

	void reveal() override;
};


#endif
