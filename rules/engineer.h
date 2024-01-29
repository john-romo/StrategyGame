// eingineer.h


#ifndef _ENGINEER_H
#define _EINGINEER_H

#include "defaults.h"
#include "piece.h"
#include "board.h"

class Engineer : public Piece{
public:
	Engineer(u8 _color);

	void reveal() override;
};



#endif
