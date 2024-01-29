// guard.cpp


#include "guard.h"

Guard::Guard(u8 _color) : Piece(GUARD, _color,
								GUARD_STAMINA_DRAIN, GUARD_HEALTH_DAMAGE, 
								GUARD_ARMOR_DAMAGE)
{
}

void Guard::reveal(){
	this->unreveal();

	Square* square = (Square*) this->square;	
	bool color = this->color;
	Heading heading = this->heading;
	std::vector<void*>* vect = &this->visibleSquares;
	
	Square* select = square;

	for(u8 i = 0; i < GUARD_VISION; ++i){
		if(select = move_selection(square, heading, i)) select->reveal_square(color, vect);
		if(i == GUARD_VISION - 2 && select){
			Square* lr;
			if(lr = move_selection(select, *heading.left, 1)) lr->reveal_square(color, vect);
			if(lr = move_selection(select, *heading.right, 1)) lr->reveal_square(color, vect);
		}
	}
}
