// main.cpp

#include <stdlib.h>

#include "main.h"
#include "board.h"
#include "defaults.h"
#include "pieceincludes.h"
#include "player.h"

int main(int, char**){
	start_game();
	
	King* king = new King(WHITE);

	place_piece(king, 20, 20);
	Rifleman* r = new Rifleman(WHITE);
	place_piece(r, 13, 13);

	king->move(15,20);
	printf("stamina: %d\n", king->player->stamina);

	reveal_pieces();
	print_board();

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






