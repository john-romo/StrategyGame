// rifleman.h


#ifndef _RIFLEMAN_H
#define _RIFLEMAN_H

#include "defaults.h"
#include "piece.h"
#include "board.h"

class Rifleman : public Piece{
public:
	Rifleman(u8 _color);

	void reveal() override;
};

#endif
