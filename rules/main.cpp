// main.cpp

#include <stdlib.h>

#include "main.h"
#include "board.h"
#include "player.h"
#include "test.h"
#include "default.h"

int main(int, char**){
	start_game();
	
 	printf("%s\n\n", run_tests());
	//print_board();

	end_game();
}


void start_game(){
	printf("\n");
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






