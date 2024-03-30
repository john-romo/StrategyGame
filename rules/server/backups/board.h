// board.h

#ifndef _BOARD_H
#define _BOARD_H

#include <unordered_map>

#include "default.h"
#include "heading.h"
#include "piece.h"


class Square{
public:
	const int x;
	const int y;

	Piece* piece;
	bool occupied;

	Square(int _x, int _y);
	
	void print_square();
};


Square* get_square(int x, int y);

extern std::unordered_map<uint64_t, Square*> board;

void create_board();
void delete_board();

void print_board();

#endif
