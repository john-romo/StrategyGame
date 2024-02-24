// scout.cpp


#include "scout.h"

Scout::Scout(u8 _color) : Piece(SCOUT, _color, 
							SCOUT_STAMINA_DRAIN, SCOUT_HEALTH_DAMAGE, 
							SCOUT_ARMOR_DAMAGE)
{

}

void Scout::reveal(){
	this->unreveal();

	Square* square = (Square*) this->square;
	bool color = this->color;
	Heading heading = this->heading;
	std::vector<void*>* vect = &this->visibleSquares;

	if(this->stance != STEALTH){
		line_reveal(square, heading, SCOUT_VISION, color, vect);

		Heading lr[2] = {*this->heading.left, *this->heading.right};
		for(int i = 0; i < 2; ++i){
			for(int j = 0, r = SCOUT_VISION-1; j < SCOUT_VISION; ++j, --r){
				if(!(square = move_selection(square, lr[i], 1))) break;
				line_reveal(square, heading, r, color, vect);
			}
			square = (Square*) this->square;
		}	
	}
	else{
		for(int i = 0; i < 8; ++i){
			Square* select = square;
			if(!(select = move_selection(square, headings[i], 1))) continue;
			select->reveal(color, vect);
		}
	}

}
