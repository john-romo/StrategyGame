// piece.cpp


#include "piece.h"
#include "board.h"
#include "pieceincludes.h"


std::vector<Piece*> whitePieces;
std::vector<Piece*> blackPieces;


Piece::Piece(
				u8 _type,
				u8 _color, 
				float _staminaDrain,	
				u8 _healthDamage, 
				u8 _armorDamage
			) : 
				type(_type), 
				color(_color), 
				staminaDrain(_staminaDrain), 
				healthDamage(_healthDamage),
				armorDamage(_armorDamage)
{
	this->x = 0;
	this->y = 0;
	this->stance = DEFEND;
	if(!this->color){
		this->player = playerWhite;
		this->heading = NORTH;
	}
	else{
		this->player = playerBlack;
		this->heading = SOUTH;
	}
	this->square = NULL;
	this->targetSquare = NULL;
}


void Piece::reveal(){}
void Piece::change_target(u8 x, u8 y){}


void Piece::unreveal(){
	for(void* square : this->visibleSquares){
		square_unreveal(square, this->color);
	}
	this->visibleSquares.clear();
}
			

bool Piece::remove_piece(){
	Square* square = (Square*) this->square;
	square->occupied = false;
	square->piece = NULL;
	this->square = NULL;
	return true;
}


bool Piece::move(u8 x, u8 y){
	Square* s = get_square(x, y);
	if(!s) return false;
	if(s->occupied) return false;
	if(!this->check_move(x, y)) return false;
	if(!this->player->drain_stamina(this->staminaDrain * this->get_distance(x, y)))
		return false;

	this->remove_piece();
	s->piece = this;
	s->occupied = true;
	this->x = s->x;
	this->y = s->y;
	this->square = s;

	return true;
}


bool Piece::check_move(u8 x, u8 y){
	if(this->x == x) return true;
	if(this->y == y) return true;
	if(abs(this->x - x) == abs(this->y - y)) return true; 
	return false;
}


u8 Piece::get_distance(u8 x, u8 y){
	if(this->y == y) return(abs(this->x - x));
	else return(abs(this->y - y));
}


bool place_piece(Piece* piece, u8 x, u8 y){
	Square* s;
	if(!(s = get_square(x, y))) return(false);

	push_piece(piece);

	s->piece = piece;
	s->occupied = true;
	piece->x = x;
	piece->y = y;
	piece->square = s;
	
	return(true);
}


void push_piece(Piece* piece){
	if(!piece->color) whitePieces.push_back(piece);
	else blackPieces.push_back(piece);
}


void reveal_pieces(){
	for(Piece* piece: whitePieces) piece->reveal();
	for(Piece* piece: blackPieces) piece->reveal();
}


void delete_pieces(){
	for(Piece* piece : whitePieces) delete(piece);
	for(Piece* piece : blackPieces) delete(piece);
}

