// board.cpp

#include "board.h"

std::unordered_map<u16, Square*> board;

Square::Square(u8 _x, u8 _y) : x(_x), y(_y) {
	this->piece = NULL;
	this->occupied = false;
	this->visibleWhite = 0;
	this->visibleBlack = 0;

	board.insert({(_x << 8) | _y, this});
}

void Square::print_square(){
	if(this->occupied) printf("%d", this->piece->type);
	else if(this->visibleWhite && (!this->visibleBlack)) printf("+");
	else if(this->visibleBlack && (!this->visibleWhite)) printf("*");
	else if (this->visibleWhite && this->visibleBlack) printf("=");
	else printf("-");
}

void Square::reveal(u8 color, std::vector<void*>* vect){
	if(!color) ++(this->visibleWhite);
	else ++(this->visibleBlack);

	vect->push_back(this);
}

void square_unreveal(void* vSquare, u8 color){
	Square* square = (Square*) vSquare;
	if(!color) --(square->visibleWhite);
	else --(square->visibleBlack);
}

Square* line_reveal(Square* s, Heading h, u8 range, bool color, std::vector<void*>* vect){
	Square* r;
	for(int i = 0; i < range; ++i){
		s->reveal(color, vect);
		r = s;
		if(!(s = move_selection(s, h, 1))) break;
	}
	return(r);
}


Square* get_square(u8 x, u8 y){
	Square* s;
	try{
		s = board.at((x << 8) | y);
		return(s);
	}
	catch(...){
		if(DEBUG) std::cerr << "ERROR in get_square() --> INVALID KEY\n";
		return(NULL);
	}
}


Square* move_selection(Square* s, Heading h, u8 magnitude){
	u8 x = (s->x) + (magnitude*h.x);
	u8 y = (s->y) + (magnitude*h.y);

	Square* r;
	if(r = get_square(x, y)) return(r);
	return(NULL);
}


void create_board(){
	for(u8 y = 0; y < DIAG - 1; ++y){
		for(u8 x = DIAG - y - 1; x < WIDTH + DIAG + y - 1; ++x){
			Square* s = new Square(x,y);
		}
	}
	for(u8 y = DIAG - 1; y < WIDTH + DIAG - 1; ++y){
		for(u8 x = 0; x < HEIGHT; ++x){
			Square* s = new Square(x,y);
		}
	}
	for(u8 y = WIDTH + DIAG - 1, d = 1; y < HEIGHT; ++y, ++d){
		for(u8 x = 0 + d; x < HEIGHT - d; ++x){
			Square* s = new Square(x,y);
		}
	}
}


void print_board(){
	Square* s;
	u8 row = 0;

	printf("%2d. ", row++);
	for(u8 y = 0; y < DIAG - 1; ++y){
		for(u8 i = DIAG - y - 2; i >= 0 && i < 255; --i){
			printf(" ");	
		}
		for(u8 x = DIAG - y - 1; x < WIDTH + DIAG + y - 1; ++x){
			if(s = get_square(x, y)) s->print_square();
		}
		printf("\n%2d. ", row++);
	}
	
	for(u8 y = DIAG - 1; y < WIDTH + DIAG - 1; ++y){
		for(u8 x = 0; x < HEIGHT; ++x){
			if(s = get_square(x, y)) s->print_square();
		}
		printf("\n%2d. ", row++);
	}
	
	for(u8 y = WIDTH + DIAG - 1, d = 1; y < HEIGHT; ++y, ++d){
		for(u8 i = 1; i <= d; ++i){
			printf(" ");
		}
		for(u8 x = 0 + d; x < HEIGHT - d; ++x){
			if(s = get_square(x, y)) s->print_square();
		}
		if(y < HEIGHT-1) printf("\n%2d. ", row++);
	}
	printf("\n");
}


void print_visible_squares(){
	printf("\nWhite Revealed Squares: \n\n");
	for(Piece* piece : whitePieces){
		printf("Type: %d\n", piece->type);
		for(void* v : piece->visibleSquares){
			Square* s = (Square*) v;
			printf("(%d, %d) -> vW: %d, vB: %d\n", s->x, s->y, s->visibleWhite, s->visibleBlack);
		}
		printf("\n");
	}
	printf("\nBlack Revealed Squares: \n\n");
	for(Piece* piece : blackPieces){
		printf("Type: %d\n", piece->type);
		for(void* v : piece->visibleSquares){
			Square* s = (Square*) v;
			printf("(%d, %d) -> vW: %d, vB: %d\n", s->x, s->y, s->visibleWhite, s->visibleBlack);
		}
		printf("\n");
	}
	printf("\n");
}


void delete_board(){
	for(auto& [key, value]: board) delete(value);
}




////////////////////////////////////////////////////////////////////
