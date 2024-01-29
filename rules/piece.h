// piece.h

#ifndef _PIECE_H
#define _PIECE_H

#include <vector>

#include "defaults.h"
#include "headings.h"
#include "player.h"

class Piece{
public:
	const u8 type;

	Player* player;
	const u8 color;

	const float staminaDrain;

	const u8 healthDamage;
	const u8 armorDamage;

	u8 health;
	u8 armor;
	 
	u8 x;
	u8 y;

	Heading heading;	

	u8 stance;

	
	void* square; 
	void* targetSquare;

	std::vector<void*> visibleSquares;
	std::vector<void*> targetableSquares;

	virtual void reveal();
	virtual void change_target(u8 x, u8 y);

	bool take_damage();
	bool die();

	bool move();
	bool target();
	bool attack();
	bool defend();
	bool stealth();

	bool repair();

	bool scan();
	bool orbital_strike();
	bool mobalize();

	Piece(
			u8 _type,
			u8 _color, 
			float _staminaDrain,	
			u8 _healthDamage, 
			u8 _armorDamage
		);

	void unreveal();
};

extern std::vector<Piece*> whitePieces;
extern std::vector<Piece*> blackPieces;


bool place_piece(Piece* piece, u8 x, u8 y);
void reveal_pieces();
void push_piece(Piece* piece);
void delete_pieces();



#endif
