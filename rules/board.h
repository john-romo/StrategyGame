// board.h

#ifndef _BOARD_H
#define _BOARD_H

#include <cstdio>
#include <iostream>
#include <unordered_map>

#include "default.h"
#include "heading.h"
#include "piece.h"


class Square{
public:
	const u8 x;
	const u8 y;

	Piece* piece;
	bool occupied;
	u8 visibleWhite;
	u8 visibleBlack;

	Square(u8 _x, u8 _y);
	
	void reveal(u8 color, std::vector<void*>* vect);
	void unreveal(u8 color);
	void print_square();
};

Square* get_square(u8 x, u8 y);

extern std::unordered_map<u16, Square*> board;

Square* line_reveal(Square* s, Heading h, u8 range, bool color, std::vector<void*>* vect);
Square* move_selection(Square* s, Heading h, u8 magnitude);

void create_board();
void delete_board();

void print_board();
void print_visible_squares();

#endif
