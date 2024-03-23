// board.cpp

#include "board.h"

std::unordered_map<u16, Square*> board;

/////////////////////////////////////////////////////////////////////////////

Square::Square(u8 _x, u8 _y, SDL_Renderer* renderer) : x(_x), y(_y) {
	this->piece = NULL;
	this->occupied = false;
	this->visibleWhite = 0;
	this->visibleBlack = 0;
	this->is_valid = false;

	std::string type = "rock";
    if(x == 10){
        type = "black";
    }else if((x+y)%2 == 1){
        type = "rock2";
    }
    Picture* p = new Picture(renderer, type, x, y, 32, 32);
	this->picture = p;

	board.insert({(_x << 8) | _y, this});
}

void Square::reveal(u8 color, std::vector<void*>* vect){
	if(!color) ++(this->visibleWhite);
	else ++(this->visibleBlack);

	vect->push_back(this);
}

void Square::unreveal(u8 color){
	if(!color) --(this->visibleWhite);
	else --(this->visibleBlack);
}

void Square::print_square(){
	if(this->occupied) printf("%d", this->piece->type);
	else if(this->visibleWhite && (!this->visibleBlack)) printf("+");
	else if(this->visibleBlack && (!this->visibleWhite)) printf("*");
	else if (this->visibleWhite && this->visibleBlack) printf("=");
	else printf("-");
}

void Square::render(int xmod, int ymod){
	picture->rect->x = xmod*32;
    picture->rect->y = ymod*32;
    SDL_RenderCopy(picture->renderer, picture->texture, NULL, picture->rect );
	return;
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


/////////////////////////////////////////////////////////////////////////////

Square* line_reveal(Square* s, Heading h, u8 range, bool color, std::vector<void*>* vect){
	Square* r;
	for(int i = 0; i < range; ++i){
		s->reveal(color, vect);
		r = s;
		if(!(s = move_selection(s, h, 1))) break;
	}
	return(r);
}

Square* move_selection(Square* s, Heading h, u8 magnitude){
	u8 x = (s->x) + (magnitude*h.x);
	u8 y = (s->y) + (magnitude*h.y);

	Square* r;
	if(r = get_square(x, y)) return(r);
	return(NULL);
}


////////////////////////////////////////////////////////////////////

void create_board(SDL_Renderer* renderer){
	for(u8 y = 0; y < DIAG - 1; ++y){
		for(u8 x = DIAG - y - 1; x < WIDTH + DIAG + y - 1; ++x){
			std::cout << static_cast<unsigned>(x) << ',' << static_cast<unsigned>(y) << std::endl;
			Square* s = new Square(x,y,renderer);
		}
	}
	for(u8 y = DIAG - 1; y < WIDTH + DIAG - 1; ++y){
		for(u8 x = 0; x < HEIGHT; ++x){
			std::cout << static_cast<unsigned>(x) << ',' << static_cast<unsigned>(y) << std::endl;
			Square* s = new Square(x,y,renderer);
		}
	}
	for(u8 y = WIDTH + DIAG - 1, d = 1; y < HEIGHT; ++y, ++d){
		for(u8 x = 0 + d; x < HEIGHT - d; ++x){
			std::cout << static_cast<unsigned>(x) << ',' << static_cast<unsigned>(y) << std::endl;
			Square* s = new Square(x,y,renderer);
		}
	}
}

void create_board_filled(SDL_Renderer* renderer){
	for(u8 y = 0; y < HEIGHT; ++y){
		for(u8 x = 0; x < HEIGHT; ++x){
			//std::cout << static_cast<unsigned>(x) << ',' << static_cast<unsigned>(y) << std::endl;
			Square* s = new Square(x,y,renderer);
			Picture* p = get_square(x,y)->picture;
			get_square(x,y)->picture = nullptr;
			p->free_picture();
			std::string type = "rock";
			
			if((x+y)%2 == 1){
				type = "rock2";
			}
			get_square(x,y)->picture = new Picture(renderer, type, 0, 0, 32, 32);
		}
	}
}

void mark_valid_tiles(SDL_Renderer* renderer){
	for(u8 y = 0; y < DIAG - 1; ++y){
		for(u8 x = DIAG - y - 1; x < WIDTH + DIAG + y - 1; ++x){
			std::cout << static_cast<unsigned>(x) << ',' << static_cast<unsigned>(y) << std::endl;
			get_square(x,y)->is_valid = true;
			// Picture* p = get_square(x,y)->picture;
			// get_square(x,y)->picture = nullptr;
			// p->free_picture();
			// get_square(x,y)->picture = new Picture(renderer, "black", 0, 0, 32, 32);
		}
	}
	for(u8 y = DIAG - 1; y < WIDTH + DIAG - 1; ++y){
		for(u8 x = 0; x < HEIGHT; ++x){
			std::cout << static_cast<unsigned>(x) << ',' << static_cast<unsigned>(y) << std::endl;
			//Square* s = new Square(x,y,renderer);
			get_square(x,y)->is_valid = true;
			// get_square(x,y)->picture->free_picture();
			// get_square(x,y)->picture = new Picture(renderer, "black", 0, 0, 32, 32);
		}
	}
	for(u8 y = WIDTH + DIAG - 1, d = 1; y < HEIGHT; ++y, ++d){
		for(u8 x = 0 + d; x < HEIGHT - d; ++x){
			std::cout << static_cast<unsigned>(x) << ',' << static_cast<unsigned>(y) << std::endl;
			// Square* s = new Square(x,y,renderer);
			get_square(x,y)->is_valid = true;
			// get_square(x,y)->picture->free_picture();
			// get_square(x,y)->picture = new Picture(renderer, "black", 0, 0, 32, 32);
		}
	}

	for(u8 y = 0; y < HEIGHT; ++y){
		for(u8 x = 0; x < HEIGHT; ++x){
			if(get_square(x,y)->is_valid == false){
				get_square(x,y)->picture->free_picture();
				get_square(x,y)->picture = new Picture(renderer, "black", 0, 0, 32, 32);
			}
		}
	}

}


void delete_board(){
	for(auto& [key, value]: board) delete(value);
}


////////////////////////////////////////////////////////////////////

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
	for(u8 i = 0; i < NUM_COLORS; ++i){
		for(Piece* piece : pieces[i]){
			printf("Type: %d\n", piece->type);
			for(void* v : piece->visibleSquares){
				Square* s = (Square*) v;
				printf("(%d, %d) -> vW: %d, vB: %d\n", s->x, s->y, s->visibleWhite, s->visibleBlack);
			}
			printf("\n");
		}
	}
	printf("\n");
}

void camera_display(int current_x, int current_y,SDL_Rect* mouse_pos, bool pressed){
	for(int y=current_y; y<current_y+15; y++){
        for(int x=current_x; x<current_x+20; x++){
			Square* s = get_square(x,y);
            s->render(x-current_x,y-current_y);
			if(s->occupied){ // if there's a piece on this square, render that too
				s->piece->button->update_button(pressed, mouse_pos);
				s->piece->button->render(x-current_x,y-current_y);
			}
        }
    }
	//std::cout << "Another frame displayed\n";
	return;
}

