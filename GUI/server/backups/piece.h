// piece.h

#ifndef _PIECE_H
#define _PIECE_H

#include <queue>
#include <vector>
#include <mutex>

#include "default.h"
#include "heading.h"

class Piece{
public:
	int id;
	const int type;
	const int color;

	int health;
	int armor;
	 
	bool placed;
	int x;
	int y;

	Heading heading;	

	int stance;
	
	void* square;
	void* targetSquare;
	
	Piece(int _type, int _color);

	bool set_stance(int s);
	bool turn(Heading h);
	bool set_target(int x, int y);
	bool move(Heading h);

	bool remove();

};



extern std::vector<Piece*> pieces[NUM_COLORS];

Piece* create_piece(int type, int _color);
void push_piece(Piece* piece);
bool place_piece(Piece* piece, int x, int y);


void delete_piece(Piece* p);
void delete_pieces();

void print_pieces();


#endif
