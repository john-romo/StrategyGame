// eingineer.cpp


#include "engineer.h"

Engineer::Engineer(u8 _color) : Piece(ENGINEER, _color, 
							ENGINEER_STAMINA_DRAIN, ENGINEER_HEALTH_DAMAGE, 
							ENGINEER_ARMOR_DAMAGE)
{

}


void Engineer::reveal(){
	this->unreveal();

	Square* square = (Square*) this->square;	
	bool color = this->color;
	Heading heading = this->heading;
	std::vector<void*>* vect = &this->visibleSquares;
	
	Square* select = square;

	for(u8 i = 0; i < ENGINEER_VISION; ++i){
		if(select = move_selection(square, heading, i)) select->reveal_square(color, vect);
		if(i == ENGINEER_VISION - 2 && select){
			Square* lr;
			if(lr = move_selection(select, *heading.left, 1)) lr->reveal_square(color, vect);
			if(lr = move_selection(select, *heading.right, 1)) lr->reveal_square(color, vect);
		}
	}
}

