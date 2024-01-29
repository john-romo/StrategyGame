// board.h

#ifndef _BOARD_H
#define _BOARD_H

#include <cstdio>
#include <iostream>
#include <unordered_map>

#include "defaults.h"
#include "headings.h"
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
	
	void print_square();
	void reveal_square(u8 color, std::vector<void*>* vect);

};

void square_unreveal(void* vSquare, u8 color);

Square* get_square(u8 x, u8 y);

u8 get_distance(Square* start, Square* end);

Square* line_reveal(Square* s, Heading h, u8 range, bool color, std::vector<void*>* vect);

Square* move_selection(Square* s, Heading h, u8 magnitude);

extern std::unordered_map<u16, Square*> board;

void create_board();
void print_board();
void print_visible_squares();
void delete_board();

#endif
