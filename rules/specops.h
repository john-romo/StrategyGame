// specops.h


#ifndef _SPECOPS_H
#define _SPECOPS_H

#include "defaults.h"
#include "piece.h"
#include "board.h"

class Specops : public Piece{
public:
	Specops(u8 _color);
	
	void reveal() override;
};



#endif
