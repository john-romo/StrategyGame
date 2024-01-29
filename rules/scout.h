// scout.h


#ifndef _SCOUT_H
#define _SCOUT_H

#include "defaults.h"
#include "piece.h"
#include "board.h"

class Scout : public Piece{
public:
	Scout(u8 _coljor);

	void reveal() override;

};



#endif
