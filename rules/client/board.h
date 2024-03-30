// board.h

#ifndef _BOARD_H
#define _BOARD_H

#include <unordered_map>

#include "../default.h"
#include "heading.h"
#include "piece.h"


class Square{
public:
	const int x;
	const int y;

	Piece* piece;
	bool occupied;
	int visibleWhite;
	int visibleBlack;

	Square(int _x, int _y);
	
	void reveal(int color, std::vector<void*>* vect);
	void unreveal(int color);
	void print_square();
};


Square* get_square(int x, int y);

extern std::unordered_map<uint64_t, Square*> board;

Square* line_reveal(Square* s, Heading h, int range, bool color, std::vector<void*>* vect);
Square* move_selection(Square* s, Heading h, int magnitude);

void create_board();
void delete_board();

void print_board();
void print_visible_squares();

#endif
