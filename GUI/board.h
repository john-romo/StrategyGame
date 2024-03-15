// board.h

#ifndef _BOARD_H
#define _BOARD_H

#include <cstdio>
#include <iostream>
#include <unordered_map>

#include "default.h"
#include "heading.h"
#include "piece.h"

#include "picture.hpp"
#include "picture.hpp"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdexcept>


class Square{
public:
	const u8 x;
	const u8 y;

	Piece* piece;
	bool occupied;
	bool is_valid;
	u8 visibleWhite;
	u8 visibleBlack;

	Square(u8 _x, u8 _y, SDL_Renderer* renderer);
	
	void reveal(u8 color, std::vector<void*>* vect);
	void unreveal(u8 color);
	void print_square();

	// extras (from merging the Tile class)
	Picture* picture = nullptr;
	void render(int xmod, int ymod);


};

Square* get_square(u8 x, u8 y);

extern std::unordered_map<u16, Square*> board;

Square* line_reveal(Square* s, Heading h, u8 range, bool color, std::vector<void*>* vect);
Square* move_selection(Square* s, Heading h, u8 magnitude);

void create_board(SDL_Renderer* renderer);
void delete_board();

void print_board();
void print_visible_squares();
void camera_display(int current_x, int current_y);
void create_board_filled(SDL_Renderer* renderer);
void mark_valid_tiles(SDL_Renderer* renderer);

#endif
