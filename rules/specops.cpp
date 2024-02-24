// king.cpp


#include "specops.h"

Specops::Specops(u8 _color) : Piece(SPECOPS, _color, 
							SPECOPS_STAMINA_DRAIN, SPECOPS_HEALTH_DAMAGE, 
							SPECOPS_ARMOR_DAMAGE)
{

}


void Specops::reveal(){
	this->unreveal();

	Square* square = (Square*) this->square;	
	bool color = this->color;
	Heading heading = this->heading;
	std::vector<void*>* vect = &this->visibleSquares;
	
	square->reveal(color, vect);
	
	Square* select = square;

	for(u8 i = 1; i < SPECOPS_VISION; ++i){
		if(select = move_selection(square, heading, i)) select->reveal(color, vect);
		if((i == SPECOPS_VISION - 2) && select){
			Square* lr;
			if(lr = move_selection(select, *heading.left, 1)) lr->reveal(color, vect);
			if(lr = move_selection(select, *heading.right, 1)) lr->reveal(color, vect);
		}

	}



}
