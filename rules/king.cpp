// king.cpp


#include "king.h"

King::King(u8 _color) : Piece(KING, _color, 
							KING_STAMINA_DRAIN, KING_HEALTH_DAMAGE, 
							KING_ARMOR_DAMAGE)
{
}


void King::reveal(){
	this->unreveal();

	Square* square = (Square*) this->square;
	bool color = this->color;
	Heading heading = this->heading;
	std::vector<void*>* vect = &this->visibleSquares;
	
	u8 range = KING_VISION;

	Square* start;
	if(!(start = move_selection(square, *heading.reverse, 1))){
		start = square;
		--range;
	}

	Heading direction;
	if(this->x < HEIGHT/2) direction = EAST;
	else direction = WEST;
	
	start = move_selection(start, direction, KING_VISION/2);

	for(int i = 0; i < range; ++i){
		line_reveal(start, *direction.reverse, KING_VISION, color, vect);
		if(!(start = move_selection(start, heading, 1))) break;
	}
}


















