// piece.h

#ifndef _PIECE_H
#define _PIECE_H

#include <vector>

#include "defaults.h"
#include "definitions.h"
#include "headings.h"
#include "player.h"

class Piece{
public:
	u8 type;

	Player* player;
	u8 color;

	float staminaDrain;

	u8 healthDamage;
	u8 armorDamage;

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

	
	Piece(u8 _type, u8 _color, u8 _stance, float _staminaDrain, u8 _healthDamage, u8 _armorDamage, i8 _health, i8 _armor);

	void reveal();
	void unreveal();

	bool set_stance(u8 s);

	bool turn(Heading* h);

	bool move(u8 x, u8 y);
	bool inc_move(i8 x, i8 y);
	u8 get_distance(u8 x, u8 y);
	bool remove();

	bool target(u8 x, u8 y);
	bool attack();
	bool repair();
	bool scan();
	bool orbital_strike();
	bool airdrop();
	bool reassign(u8 type);

};

Piece* create_piece(u8 type, u8 _color);
extern std::vector<Piece*> whitePieces;
extern std::vector<Piece*> blackPieces;
extern u8 whitePiecesCreated[8];
extern u8 blackPiecesCreated[8];
bool inc_created_pieces(u8 type, u8 color);
void dec_created_pieces(u8 type, u8 color);


void reveal_pieces();
void king_reveal(Piece* p);
void guard_reveal(Piece* p);
void rifleman_reveal(Piece* p);
void specops_reveal(Piece* p);
void paratrooper_reveal(Piece* p);
void engineer_reveal(Piece* p);
void scout_reveal(Piece* p);
void searchlight_reveal(Piece* p);

bool searchlight_target(Piece* p, u8 y);

bool place_piece(Piece* piece, u8 x, u8 y);
void push_piece(Piece* piece);
void print_pieces();
void delete_piece(Piece* p);
void delete_pieces();



#endif
