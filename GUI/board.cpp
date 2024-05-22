// board.cpp

#include "board.h"

std::unordered_map<uint64_t, Square*> board;

/////////////////////////////////////////////////////////////////////////////

Square::Square(int _x, int _y, SDL_Renderer* renderer) : x(_x), y(_y) {
	this->piece = NULL;
	this->occupied = false;
	this->visibleWhite = 0;
	this->visibleBlack = 0;
	this->is_valid = false;

	uint64_t top = _x;

	std::string type = "rock";
    if(x == 10){
        type = "black";
    }else if((x+y)%2 == 1){
        type = "rock2";
    }
    Picture* p = new Picture(renderer, type, x, y, 32, 32);
	this->picture = p;

	board.insert({(top << (sizeof(int)*8)) | _y, this});
}

void Square::reveal(int color, std::vector<void*>* vect){
	if(!color) ++(this->visibleWhite);
	else ++(this->visibleBlack);

	vect->push_back(this);
}

void Square::unreveal(int color){
	if(!color) --(this->visibleWhite);
	else --(this->visibleBlack);
}

void Square::print_square(){
	if(this->occupied){
		if(this->piece->stance == STEALTH) printf("s");
		else printf("%d", this->piece->type);
	}
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


Square* get_square(int x, int y){ //TODO CHECK THIS
	Square* s;
	try{
		uint64_t top = x;

		s = board.at((top << (sizeof(int)*8)) | y);
		return(s);
	}
	catch(...){
		printf("Error: returned null square\n");
		return(NULL);
	}
}

Square* get_square_old(int x, int y){
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

Square* line_reveal(Square* s, Heading h, int range, bool color, std::vector<void*>* vect){
	Square* r;
	for(int i = 0; i < range; ++i){
		s->reveal(color, vect);
		r = s;
		if(!(s = move_selection(s, h, 1))) break;
	}
	return(r);
}

Square* move_selection(Square* s, Heading h, int magnitude){
	int x = (s->x) + (magnitude*h.x);
	int y = (s->y) + (magnitude*h.y);

	Square* r;
	if(r = get_square(x, y)) return(r);
	return(NULL);
}


////////////////////////////////////////////////////////////////////

void create_board(SDL_Renderer* renderer){
	for(int y = 0; y < DIAG - 1; ++y){
		for(int x = DIAG - y - 1; x < WIDTH + DIAG + y - 1; ++x){
			//Square* s = new Square(x,y);
			Square* s = new Square(x,y,renderer);
		}
	}
	for(int y = DIAG - 1; y < WIDTH + DIAG - 1; ++y){
		for(int x = 0; x < HEIGHT; ++x){
			//Square* s = new Square(x,y);
			Square* s = new Square(x,y,renderer);
		}
	}
	for(int y = WIDTH + DIAG - 1, d = 1; y < HEIGHT; ++y, ++d){
		for(int x = 0 + d; x < HEIGHT - d; ++x){
			//Square* s = new Square(x,y);
			Square* s = new Square(x,y,renderer);
		}
	}
}

// This creates a "filled" board that creates Squares in all spaces,
// (including outside the octogon). The dimensions will be HEIGHTxHEIGHT.
// Invalid Squares (those outside the octogon) are marked by the
// mark_valid_tiles function.

void create_board_filled(SDL_Renderer* renderer){
	for(int y = 0; y < HEIGHT; ++y){
		for(int x = 0; x < HEIGHT; ++x){
			Square* s = new Square(x,y,renderer);
			Picture* p = get_square(x,y)->picture;
			get_square(x,y)->picture = nullptr;
			p->free_picture();
			std::string type = "grass4"; // this is the PNG name for the image.
			
			if((x+y)%2 == 1){
				type = "grass4";
			}else{
				;
				// this is now empty, but changing the "type" string
				// in this block will create a checkerboard.
			}
			// Create and assign a picture to the square.
			//std::cout << "going to free..." << std::endl;
			delete get_square(x,y)->picture;
			get_square(x,y)->picture = new Picture(renderer, type, 0, 0, 32, 32);
			//std::cout << x << y << std::endl;
		}
	}
}

// This will mark tiles as "valid" if they are within the area
// of the octogon.

void mark_valid_tiles(SDL_Renderer* renderer){
	for(int y = 0; y < DIAG - 1; ++y){
		for(int x = DIAG - y - 1; x < WIDTH + DIAG + y - 1; ++x){
			get_square(x,y)->is_valid = true;
		}
	}
	for(int y = DIAG - 1; y < WIDTH + DIAG - 1; ++y){
		for(int x = 0; x < HEIGHT; ++x){
			get_square(x,y)->is_valid = true;
		}
	}
	for(int y = WIDTH + DIAG - 1, d = 1; y < HEIGHT; ++y, ++d){
		for(int x = 0 + d; x < HEIGHT - d; ++x){
			get_square(x,y)->is_valid = true;
		}
	}

	// Tiles that are invalid (not within the octogon) will have its picture changed.
	for(int y = 0; y < HEIGHT; ++y){
		for(int x = 0; x < HEIGHT; ++x){
			if(get_square(x,y)->is_valid == false){
				get_square(x,y)->picture->free_picture(); // free the old picture
				delete get_square(x,y)->picture;
				// create and assign new picture. "rockdark" is the name of the PNG.
				get_square(x,y)->picture = new Picture(renderer, "rockdark", 0,0,32,32); 
			}
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


void print_visible_squares(){
	printf("\nWhite Revealed Squares: \n\n");
	for(int i = 0; i < NUM_COLORS; ++i){
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

// The function that shows and updates the map every frame. 
// Arguments current_x and current_y are provided by the Camera class
// and this indicates where in the map the player is. (the square in the top left corner in the player's view.)


void camera_display(int current_x, int current_y, SDL_Rect* mouse_pos, bool pressed, int color, Picture* blank){
	// for every square in the player's view:
	for(int y=current_y; y<current_y+15; y++){
        for(int x=current_x; x<current_x+20; x++){
			Square* s = get_square(x,y);
			// if the square is visible to this client, show the square
			if((color == 1 && s->visibleBlack) || (color == 0 && s->visibleWhite) || s->is_valid == false){
				s->render(x-current_x,y-current_y);
				if(s->occupied){ // if there's a piece on this square, render that too
					s->piece->button->update_button(pressed, mouse_pos);
					if(s->piece->is_selected){
						s->piece->button->current_picture = s->piece->button->clicked_picture;
					}
					s->piece->button->render(x-current_x,y-current_y, !(s->piece->is_selected));
				}
			}else{ // otherwise show a blank
				blank->rect->x = (x-current_x)*32;
				blank->rect->y = (y-current_y)*32;
				SDL_RenderCopy(blank->renderer, blank->texture, NULL, blank->rect );
			}
        }
    }
	return;
}

// Function for updating each client's board after the server sends over the 
// new pieces placed. It goes through the board and checks for the newly added pieces. 
// Then, it creates and assigns a button to each new piece so it can be seen.

void update_board(SDL_Renderer* renderer){
	for(int y = 0; y < HEIGHT; ++y){
		for(int x = 0; x < HEIGHT; ++x){
			Square* s = get_square(x,y);
			if(s->occupied){
				if(s->piece->button == nullptr)
					if(s->piece->type == KING) s->piece->button = new Button2(renderer, "king", 0,0,32,32);
					else if(s->piece->type == ENGINEER) s->piece->button = new Button2(renderer, "engineer", 0,0,32,32);
					else if(s->piece->type == GUARD) s->piece->button = new Button2(renderer, "guard", 0,0,32,32);
					else if(s->piece->type == SCOUT) s->piece->button = new Button2(renderer, "scout", 0,0,32,32);
					else if(s->piece->type == SEARCHLIGHT) s->piece->button = new Button2(renderer, "searchlight", 0,0,32,32);
					else if(s->piece->type == RIFLEMAN) s->piece->button = new Button2(renderer, "rifleman", 0,0,32,32);
					else if(s->piece->type == SPECOPS) s->piece->button = new Button2(renderer, "specops", 0,0,32,32);
					else if(s->piece->type == PARATROOPER) s->piece->button = new Button2(renderer, "paratrooper", 0,0,32,32);
			}
		}
	}
}

