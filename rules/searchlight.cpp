// searchlight.cpp


#include "searchlight.h"

Searchlight::Searchlight(u8 _color) : Piece(SEARCHLIGHT, _color, 
							SEARCHLIGHT_STAMINA_DRAIN, SEARCHLIGHT_HEALTH_DAMAGE, 
							SEARCHLIGHT_ARMOR_DAMAGE)
{

}

void Searchlight::reveal(){
	this->unreveal();

	Square* square = (Square*) this->targetSquare;
	if(!square) return;
	bool color = this->color;
	std::vector<void*>* vect = &this->visibleSquares;
	Heading vertical, horizontal;

	if(square->y < HEIGHT/2) vertical = NORTH;
	else vertical = SOUTH;
	square = move_selection(square, vertical, SEARCHLIGHT_VISION/2);

	if(square->x < HEIGHT/2) horizontal = EAST;
	else horizontal = WEST;
	square = move_selection(square, horizontal, SEARCHLIGHT_VISION/2);

	Square* start = square;
	for(int i = 0; i < SEARCHLIGHT_VISION; ++i){
		line_reveal(start, *horizontal.reverse, SEARCHLIGHT_VISION, color, vect);
		if(!(start = move_selection(start, *vertical.reverse, 1))) break;
	}
}

void Searchlight::change_target(u8 x, u8 y){
	Square* target;
	if(!(target = get_square(x, y))) return;	
	this->targetSquare = target;
}
