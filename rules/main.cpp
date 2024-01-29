// main.cpp

#include <stdlib.h>

#include "main.h"
#include "board.h"
#include "defaults.h"
#include "pieceincludes.h"
#include "player.h"

int main(int, char**){
	start_game();
	
	/*
	Specops* s = new Specops(BLACK);
	s->heading = SOUTHWEST;
	place_piece(s, 18, 18);

	King* k = new King(WHITE);
	place_piece(k, 18, 4);

	Rifleman* r = new Rifleman(WHITE);
	place_piece(r, 17, 5);
	*/

	/*
	Searchlight* sl1 = new Searchlight(WHITE);
	place_piece(sl1, 9, 0);
	sl1->change_target(25, 30);

	Searchlight* sl2 = new Searchlight(BLACK);
	place_piece(sl2, 22, 30);
	sl2->change_target(18, 18);

	Searchlight* sl = new Searchlight(WHITE);
	place_piece(sl, 15, 4);
	sl->change_target(22, 17);


	reveal_pieces();
	//print_visible_squares();
	print_board();
	*/

	Square* start = new Square(1, 1);
	Square* end = new Square(18, 27);
	u8 distance = get_distance(start, end);
	
	printf("%d\n", distance);

	end_game();
}


void start_game(){
	create_players();
	create_headings();
	create_board();


}

void end_game(){
	delete_pieces();
	delete_board();
	delete_players();
	printf("\n");
}






