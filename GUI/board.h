// board.h

#ifndef _BOARD_H
#define _BOARD_H

#include <iostream>
#include <unordered_map>

#include "default.h"
#include "heading.h"
#include "piece.h"

#include "picture.hpp"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdexcept>


class Square{
public:
	const int x;
	const int y;

	Piece* piece;
	bool occupied;
	bool is_valid;
	int visibleWhite;
	int visibleBlack;

	Square(int _x, int _y, SDL_Renderer* renderer);
	

	void reveal(int color, std::vector<void*>* vect);
	void unreveal(int color);
	void print_square();

	// extras (from merging the Tile class)
	Picture* picture = nullptr;
	void render(int xmod, int ymod);
};


Square* get_square(int x, int y);
Square* get_square_old(int x, int y);

extern std::unordered_map<uint64_t, Square*> board;

Square* line_reveal(Square* s, Heading h, int range, bool color, std::vector<void*>* vect);
Square* move_selection(Square* s, Heading h, int magnitude);

void create_board(SDL_Renderer* renderer);
void delete_board();

void print_board();
void print_visible_squares();

void camera_display(int current_x, int current_y, SDL_Rect* mouse_pos, bool pressed, int color, Picture* blank);
void create_board_filled(SDL_Renderer* renderer);
void mark_valid_tiles(SDL_Renderer* renderer);
void update_board(SDL_Renderer* renderer);

#endif
