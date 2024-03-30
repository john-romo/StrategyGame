// piece.cpp

#include "piece.h"
#include "board.h"


//////////// CREATION //////////////////////////////////////////////////////////////////////////

std::vector<Piece*> pieces[NUM_COLORS];
int ids = 0;


Piece* create_piece(int type, int color){
	if(color >= NUM_COLORS ) return NULL;
	if(type >= NUM_PIECE_TYPES) return NULL;

	switch(type){
		case KING:
			return new Piece(KING, color);
		case GUARD:
			return new Piece(GUARD, color);
		case RIFLEMAN:
			return new Piece(RIFLEMAN, color);
		case SPECOPS:
			return new Piece(SPECOPS, color);
		case PARATROOPER:
			return new Piece(PARATROOPER, color);
		case ENGINEER:
			return new Piece(ENGINEER, color);
		case SCOUT:
			return new Piece(SCOUT, color);
		case SEARCHLIGHT:
			return new Piece(SEARCHLIGHT, color);
		default:
			return NULL;
	}
}


Piece::Piece(int _type, int _color) : type(_type), color(_color){
	this->id = ids++;
	this->stance = ACTION;
	this->health = STARTING_HEALTH[_type];
	this->armor = STARTING_ARMOR[_type];
	this->x = 0;
	this->y = 0;
	this->heading = *START_HEADINGS[_color]; 
	this->square = NULL;
	this->targetSquare = NULL;
	pieces[_color].push_back(this);
}


bool place_piece(Piece* piece, int x, int y){
	if(piece->placed) return false;
	Square* s;
	if(!(s = get_square(x, y))) return false;
	if(s->occupied) return false;

	piece->remove();
	s->piece = piece;
	s->occupied = true;
	piece->x = x;
	piece->y = y;
	piece->square = s;

	return true;
}


//////////// ACTIONS ///////////////////////////////////////////////////////////////////////////

bool Piece::set_stance(int s){
	this->stance = s;
	return true;
}


bool Piece::turn(Heading h){
	this->heading = h;
	return true;
}


bool Piece::set_target(int x, int y){
	Square* ts = get_square(x, y);
	this->targetSquare = ts;
	return true;
}


bool Piece::move(Heading h){
	Square* square = get_square(this->x + h.x, this->y + h.y);
	if(square->occupied) return false;
	else place_piece(this, this->x + h.x, this->y + h.y);
	return true;
}

bool Piece::remove(){
	Square* square = (Square*) this->square;
	square->occupied = false;
	square->piece = NULL;
	this->square = NULL;
	return true;
}


//////////// DELETION //////////////////////////////////////////////////////////////////////////

void delete_piece(Piece* p){
	p->remove();
	int size = pieces[p->color].size();
	int i = 0;
	for(; i < size; i++) if(pieces[p->color][i] == p) break;
	pieces[p->color].erase(pieces[p->color].begin() + i);
	delete(p);
}


void delete_pieces(){
	for(int i = 0; i < NUM_COLORS; ++i){
		while(!pieces[i].empty()) delete_piece(pieces[i].front());
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////

void print_pieces(){
	for(int i = 0; i < NUM_COLORS; ++i){
		printf("Color %d Pieces:\n", i);
		for(Piece* piece: pieces[i]) printf("%d ", piece->type);
		printf("\n");
	}
}


