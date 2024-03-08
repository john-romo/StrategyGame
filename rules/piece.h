// piece.h

#ifndef _PIECE_H
#define _PIECE_H

#include <vector>
#include <algorithm>

#include "default.h"
#include "heading.h"
#include "player.h"

class Piece{
public:
	const u8 type;
	const u8 color;

	i8 health;
	i8 armor;
	 
	bool placed;
	u8 x;
	u8 y;

	Heading heading;	

	u8 stance;

	void* square; 
	void* targetSquare;

	std::vector<void*> visibleSquares;
	std::vector<void*> targetableSquares;
	
	Piece(u8 _type, u8 _color);

	void reveal();
	void unreveal();

	bool set_stance(u8 s);
	bool turn(Heading* h);
	bool set_target(u8 x, u8 y);

	bool move(u8 x, u8 y);
	bool remove();
	bool attack();
	bool repair();
	bool scan();
	bool orbital_strike();
	bool airdrop();
	bool reassign(u8 type);

private:
	bool inc_move(i8 x, i8 y);
	u8 get_distance(u8 x, u8 y);

	bool set_searchlight_target(u8 y);

	void king_reveal();
	void guard_reveal();
	void rifleman_reveal();
	void specops_reveal();
	void paratrooper_reveal();
	void engineer_reveal();
	void scout_reveal();
	void searchlight_reveal();
};



extern std::vector<Piece*> pieces[NUM_COLORS];
extern u8 piecesCreated[NUM_COLORS][NUM_PIECE_TYPES];
bool inc_created_pieces(u8 type, u8 color);
Piece* create_piece(u8 type, u8 _color);
void push_piece(Piece* piece);
bool place_piece(Piece* piece, u8 x, u8 y);


void reveal_pieces();

void dec_created_pieces(u8 type, u8 color);
void delete_piece(Piece* p);
void delete_pieces();

void print_pieces();


#endif
