// piece.h

#ifndef _PIECE_H
#define _PIECE_H

#include <queue>
#include <vector>
#include <mutex>

#include "default.h"
#include "heading.h"
#include "player.h"
#include "button2.hpp"

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

	std::vector<void*> visibleSquares;
	
	Piece(int _type, int _color);

	std::queue<void*> tasks;

	std::mutex mutex;
	void cancel();

	void reveal();
	void unreveal();

	bool set_stance(int s);
	bool turn(Heading* h);
	bool set_target(int x, int y);

	bool move(Heading h);
	void bump(int x, int y);
	bool remove();
	bool attack();
	bool repair();
	bool scan();
	bool orbital_strike();
	bool airdrop();
	bool reassign(int type);

	Button2* button = nullptr;
	bool is_selected = false;

private:
	int get_distance(int x, int y);

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
extern int piecesCreated[NUM_COLORS][NUM_PIECE_TYPES];
bool inc_created_pieces(int type, int color);
Piece* create_piece(int type, int _color);
void push_piece(Piece* piece);
bool place_piece(Piece* piece, int x, int y);
void default_placement();

void reveal_pieces();

void dec_created_pieces(int type, int color);
void delete_piece(Piece* p);
void delete_pieces();

void print_pieces();

void create_pieces();

#endif
