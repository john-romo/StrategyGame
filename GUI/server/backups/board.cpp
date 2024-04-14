// board.cpp

#include "board.h"

std::unordered_map<uint64_t, Square*> board;

/////////////////////////////////////////////////////////////////////////////

Square::Square(int _x, int _y) : x(_x), y(_y) {
	this->piece = NULL;
	this->occupied = false;

	uint64_t top = _x;
	board.insert({(top << (sizeof(int)*8)) | _y, this});
}


void Square::print_square(){
	if(this->occupied){
		if(this->piece->stance == STEALTH) printf("s");
		else printf("%d", this->piece->type);
	}
	else printf("-");
}


Square* get_square(int x, int y){
	Square* s;
	try{
		uint64_t top = x;

		s = board.at((top << (sizeof(int)*8)) | y);
		return(s);
	}
	catch(...){
		return(NULL);
	}
}


/////////////////////////////////////////////////////////////////////////////

void create_board(){
	for(int y = 0; y < DIAG - 1; ++y){
		for(int x = DIAG - y - 1; x < WIDTH + DIAG + y - 1; ++x){
			Square* s = new Square(x,y);
		}
	}
	for(int y = DIAG - 1; y < WIDTH + DIAG - 1; ++y){
		for(int x = 0; x < HEIGHT; ++x){
			Square* s = new Square(x,y);
		}
	}
	for(int y = WIDTH + DIAG - 1, d = 1; y < HEIGHT; ++y, ++d){
		for(int x = 0 + d; x < HEIGHT - d; ++x){
			Square* s = new Square(x,y);
		}
	}
}


void delete_board(){
	for(auto& [key, value]: board){
		delete(value);
	}
}


////////////////////////////////////////////////////////////////////

void print_board(){
	Square* s;
	int row = 0;

	printf("%2d. ", row++);
	for(int y = 0; y < DIAG - 1; ++y){
		for(int i = DIAG - y - 2; i >= 0 && i < 255; --i){
			printf(" ");	
		}
		for(int x = DIAG - y - 1; x < WIDTH + DIAG + y - 1; ++x){
			if(s = get_square(x, y)) s->print_square();
		}
		printf("\n%2d. ", row++);
	}
	
	for(int y = DIAG - 1; y < WIDTH + DIAG - 1; ++y){
		for(int x = 0; x < HEIGHT; ++x){
			if(s = get_square(x, y)) s->print_square();
		}
		printf("\n%2d. ", row++);
	}
	
	for(int y = WIDTH + DIAG - 1, d = 1; y < HEIGHT; ++y, ++d){
		for(int i = 1; i <= d; ++i){
			printf(" ");
		}
		for(int x = 0 + d; x < HEIGHT - d; ++x){
			if(s = get_square(x, y)) s->print_square();
		}
		if(y < HEIGHT-1) printf("\n%2d. ", row++);
	}
	printf("\n\n");
}



