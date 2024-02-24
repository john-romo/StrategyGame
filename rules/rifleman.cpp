// rifleman.cpp


#include "rifleman.h"

Rifleman::Rifleman(u8 _color) : Piece(RIFLEMAN, _color, 
							RIFLEMAN_STAMINA_DRAIN, RIFLEMAN_HEALTH_DAMAGE, 
							RIFLEMAN_ARMOR_DAMAGE)
{

}

void Rifleman::reveal(){
	this->unreveal();

	Square* square = (Square*) this->square;
	Heading heading = this->heading;
	u8 color = this->color;
	std::vector<void*>* vect = &this->visibleSquares;
	
	line_reveal(square, heading, RIFLEMAN_VISION, color, vect);

	Square* select;
	if(select = move_selection(square, *heading.right, 1))
		select->reveal(color, vect);
	
	if(select = move_selection(square, *heading.left, 1))
		select->reveal(color, vect);
}
