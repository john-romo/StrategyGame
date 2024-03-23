// piece.cpp

#include "piece.h"
#include "board.h"


//////////// CREATION //////////////////////////////////////////////////////////////////////////

std::vector<Piece*> pieces[NUM_COLORS];
u8 piecesCreated[NUM_COLORS][NUM_PIECE_TYPES] = {0};


Piece* create_piece(u8 type, u8 color){
	if(color > BLACK) return NULL;
	if(type > (NUM_PIECE_TYPES-1)) return NULL;
	if((piecesCreated[color][type] + 1) > MAX_PIECES_CREATED[color][type]) return NULL;
	else ++piecesCreated[color][type];
	switch(type){
		case KING:
			return new Piece(KING, color);
		case GUARD:
			return new Piece(GUARD, color);
		case RIFLEMAN:
			return new Piece(RIFLEMAN, color);
		case SPECOPS:
			return new Piece(SPECOPS, color);
		case PARATROOPER:
			return new Piece(PARATROOPER, color);
		case ENGINEER:
			return new Piece(ENGINEER, color);
		case SCOUT:
			return new Piece(SCOUT, color);
		case SEARCHLIGHT:
			return new Piece(SEARCHLIGHT, color);
		default:
			return NULL;
	}
}


Piece::Piece(u8 _type, u8 _color) : type(_type), color(_color){
	this->stance = ACTION;
	this->health = STARTING_HEALTH[_type];
	this->armor = STARTING_ARMOR[_type];
	this->placed = false;
	this->x = 0;
	this->y = 0;
	this->heading = *START_HEADINGS[_color]; 
	this->square = NULL;
	this->targetSquare = NULL;
	pieces[_color].push_back(this);

	std::string button_type = "pawn";

	if(_type == RIFLEMAN){
		button_type = "warning";
	}
	
}



bool place_piece(Piece* piece, u8 x, u8 y){
	if(piece->placed) return false;
	Square* s;
	if(!(s = get_square(x, y))) return false;
	if(s->occupied) return false;

	s->piece = piece;
	s->occupied = true;
	piece->x = x;
	piece->y = y;
	piece->square = s;
	piece->placed = true;
	//piece->reveal();

	return true;
}

//////////// VISION ////////////////////////////////////////////////////////////////////////////

void Piece::reveal(){
	switch(this->type){
		case KING:
			this->king_reveal();
			break;
		case ENGINEER:
			this->engineer_reveal();
			break;
		case SCOUT:
			this->scout_reveal();
			break;
		case SEARCHLIGHT:
			this->searchlight_reveal();
			break;
		case GUARD:
			this->guard_reveal();
			break;
		case RIFLEMAN:
			this->rifleman_reveal();
			break;
		case SPECOPS:
			this->specops_reveal();
			break;
		case PARATROOPER:
			this->paratrooper_reveal();
			break;
		default:
			break;
	}
}


void Piece::king_reveal(){
	Square* square = (Square*) this->square;
	bool color = this->color;
	Heading heading = this->heading;
	std::vector<void*>* vect = &this->visibleSquares;

	u8 range = KING_VISION;

	Square* start;
	if(!(start = move_selection(square, *(heading.reverse), 1))){
		start = square;
		--range;
	}


	Heading direction;
	if(this->x < HEIGHT/2) direction = EAST;
	else direction = WEST;
	
	start = move_selection(start, direction, KING_VISION/2);

	for(int i = 0; i < range; ++i){
		line_reveal(start, *direction.reverse, KING_VISION, color, vect);
		if(!(start = move_selection(start, heading, 1))) break;
	}
}


void Piece::guard_reveal(){
	Square* square = (Square*) this->square;	
	bool color = this->color;
	Heading heading = this->heading;
	std::vector<void*>* vect = &this->visibleSquares;
	
	Square* select = square;

	for(u8 i = 0; i < GUARD_VISION; ++i){
		if(select = move_selection(square, heading, i)) select->reveal(color, vect);
		if(i == GUARD_VISION - 2 && select){
			Square* lr;
			if(lr = move_selection(select, *heading.left, 1)) lr->reveal(color, vect);
			if(lr = move_selection(select, *heading.right, 1)) lr->reveal(color, vect);
		}
	}
}


void Piece::rifleman_reveal(){
	Square* square = (Square*) this->square;
	Heading heading = this->heading;
	u8 color = this->color;
	std::vector<void*>* vect = &this->visibleSquares;
	
	line_reveal(square, heading, RIFLEMAN_VISION, color, vect);

	Square* select;
	if(select = move_selection(square, *heading.right, 1))
		select->reveal(color, vect);
	
	if(select = move_selection(square, *heading.left, 1))
		select->reveal(color, vect);
}


void Piece::specops_reveal(){
	Square* square = (Square*) this->square;	
	bool color = this->color;
	Heading heading = this->heading;
	std::vector<void*>* vect = &this->visibleSquares;
	
	square->reveal(color, vect);
	
	Square* select = square;

	for(u8 i = 1; i < SPECOPS_VISION; ++i){
		if(select = move_selection(square, heading, i)) select->reveal(color, vect);
		if((i == SPECOPS_VISION - 2) && select){
			Square* lr;
			if(lr = move_selection(select, *heading.left, 1)) lr->reveal(color, vect);
			if(lr = move_selection(select, *heading.right, 1)) lr->reveal(color, vect);
		}
	}
}


void Piece::paratrooper_reveal(){};


void Piece::engineer_reveal(){
	Square* square = (Square*) this->square;	
	bool color = this->color;
	Heading heading = this->heading;
	std::vector<void*>* vect = &this->visibleSquares;
	
	Square* select = square;

	for(u8 i = 0; i < ENGINEER_VISION; ++i){
		if(select = move_selection(square, heading, i)) select->reveal(color, vect);
		if(i == ENGINEER_VISION - 2 && select){
			Square* lr;
			if(lr = move_selection(select, *heading.left, 1)) lr->reveal(color, vect);
			if(lr = move_selection(select, *heading.right, 1)) lr->reveal(color, vect);
		}
	}
}


void Piece::scout_reveal(){
	Square* square = (Square*) this->square;
	bool color = this->color;
	Heading heading = this->heading;
	std::vector<void*>* vect = &this->visibleSquares;

	if(this->stance != STEALTH){
		line_reveal(square, heading, SCOUT_VISION, color, vect);

		Heading lr[2] = {*this->heading.left, *this->heading.right};
		for(int i = 0; i < 2; ++i){
			for(int j = 0, r = SCOUT_VISION-1; j < SCOUT_VISION; ++j, --r){
				if(!(square = move_selection(square, lr[i], 1))) break;
				line_reveal(square, heading, r, color, vect);
			}
			square = (Square*) this->square;
		}	
	}
	else{
		for(int i = 0; i < 8; ++i){
			Square* select = square;
			if(!(select = move_selection(square, headings[i], 1))) continue;
			select->reveal(color, vect);
		}
	}
}


void Piece::searchlight_reveal(){
	Square* square = (Square*) this->targetSquare;
	if(!square) return;
	bool color = this->color;
	std::vector<void*>* vect = &this->visibleSquares;
	Heading vertical, horizontal;

	if(square->y < HEIGHT/2) vertical = NORTH;
	else vertical = SOUTH;
	square = move_selection(square, vertical, SEARCHLIGHT_VISION/2);

	if(square->x < HEIGHT/2) horizontal = EAST;
	else horizontal = WEST;
	square = move_selection(square, horizontal, SEARCHLIGHT_VISION/2);

	Square* start = square;
	for(int i = 0; i < SEARCHLIGHT_VISION; ++i){
		line_reveal(start, *horizontal.reverse, SEARCHLIGHT_VISION, color, vect);
		if(!(start = move_selection(start, *vertical.reverse, 1))) break;
	}
}


void Piece::unreveal(){
	for(void* square : this->visibleSquares){
		Square* s = (Square*) square;
		s->unreveal(this->color);
	}
	this->visibleSquares.clear();
}


void reveal_pieces(){
	for(u8 i = 0; i < NUM_COLORS; ++i){
		for(Piece* piece : pieces[i]) piece->reveal();
	}
}


//////////// MUTATIONS /////////////////////////////////////////////////////////////////////////

bool Piece::set_stance(u8 s){
	if(this->type == SEARCHLIGHT) return false;
	if(this->type == KING) return false;
	if(this->stance == s) return true;
	if((s == DEFEND) && (this->type == SCOUT)) return false;
	if((s == STEALTH) && !((this->type == SCOUT) || (this->type == SPECOPS) || (this->type == PARATROOPER))) return false;
	this->stance = s;
	return true;
}

//--------------------------------------------------------------------------------------------//

bool Piece::turn(Heading* h){
	if(!this->placed) return false;
	if((this->type == KING) || (this->type == SEARCHLIGHT)) return false;
	this->heading = *h;
	this->unreveal();
	this->reveal();
	return true;
}

//--------------------------------------------------------------------------------------------//

bool Piece::set_target(u8 x, u8 y){
	if(this->type > NUM_PIECE_TYPES-1) return false;
	if(this->type == SCOUT) return false;

	Square* ts = get_square(x, y);

	if(this->type == SEARCHLIGHT){
		bool check = this->set_searchlight_target(y);
		if(!check) return false;
	}
	if(this->type != KING){
		if(!(ts && ts->occupied)) return false;
		if(!((this->type == ENGINEER) ^ (this->color != ts->piece->color))) return false;
		bool cont = false;
		for(void* s : this->visibleSquares) if(((Square*) s) == ts) cont = true;
		if(!cont) return false;
	}

	this->targetSquare = ts;
	return true;
}

bool Piece::set_searchlight_target(u8 y){
	u8 midline = (HEIGHT-1)/2;
	if(this->color == WHITE && y < midline) return true;
	if(this->color == BLACK && y > midline) return true;
	return false;
}


//////////// ACTIONS ///////////////////////////////////////////////////////////////////////////

bool Piece::move(u8 x, u8 y){
	if(!this->placed) return false;
	if(this->type == SEARCHLIGHT) return false;
	if(this->stance != ACTION) return false;
	Square* s = get_square(x, y);
	if(!s) return false;
	if(!players[this->color]->drain_stamina(STAMINA_DRAIN[this->type] * this->get_distance(x, y)))
		return false;

	i8 incX = -1;
	i8 incY = -1;
	if(this->x == x) incX = 0;
	else if(this->y == y) incY = 0;
	else if(abs(this->x - x) != abs(this->y - y)) return false;
	if(incX) incX = (x - this->x)/(abs(x - this->x));
	if(incY) incY = (y - this->y)/(abs(y - this->y));

	while(((this->y != y) || (this->x != x)) && inc_move(incX, incY)){};

	return true;
}


bool Piece::remove(){
	if(!this->placed) return false;
	Square* square = (Square*) this->square;
	square->occupied = false;
	square->piece = NULL;
	this->square = NULL;
	this->placed = false;
	this->unreveal();
	return true;
}


bool Piece::inc_move(i8 x, i8 y){
	Square* s = get_square(this->x + x, this->y + y);
	if(s->occupied) return false;
	this->remove();
	s->piece = this;
	s->occupied = true;
	this->placed = true;
	this->x = s->x;
	this->y = s->y;
	this->square = s;
	this->reveal();
	return true;
}


u8 Piece::get_distance(u8 x, u8 y){
	if(this->y == y) return(abs(this->x - x));
	else return(abs(this->y - y));
}

//--------------------------------------------------------------------------------------------//

bool Piece::attack(){
	if(this->stance != ACTION) return false;
	if(!IS_ATTACKING_PIECE(this->type)) return false;
	if(!this->placed) return false; 
	if(!this->targetSquare) return false;
	Piece* target = ((Square*) this->targetSquare)->piece;
	u8 hd = HEALTH_DAMAGE[type];
	u8 ad = ARMOR_DAMAGE[type];
	if(target->stance == DEFEND){
		hd = DEFEND_HEALTH_DAMAGE_MOD(hd); 
		ad = DEFEND_ARMOR_DAMAGE_MOD(ad);
	}
	target->armor -= ad;
	if(target->armor < 0){
		target->armor = 0;
		hd = ZERO_ARMOR_DAMAGE_MOD(hd);
	}
	target->health -= hd;
	if(target->health <= 0){
		delete_piece(target);	
		this->targetSquare = NULL;
	}

	return true;
}

//--------------------------------------------------------------------------------------------//

bool Piece::repair(){
	if(this->stance != ACTION) return false;
	if(!this->type == ENGINEER) return false;
	if(!this->placed) return false;
	if(!this->targetSquare) return false;
	Piece* target = ((Square*) this->targetSquare)->piece;
	if(!target->armor < STARTING_ARMOR[target->type]) return false;
	++target->armor;
	return true;
}

//--------------------------------------------------------------------------------------------//

bool Piece::reassign(u8 type){
	if(this->stance != ACTION) return false;
	if(type > NUM_PIECE_TYPES-1) return false;
	if((type == SEARCHLIGHT) || (type == KING)) return false;
	Square* pos = (Square*) this->targetSquare;
	if(!pos) return false;
	if(!pos->occupied) return false;
	Piece* target = pos->piece;
	if(target->type == type) return false;
	if(target->color != this->color) return false;

	if(!(piecesCreated[color][type] < MAX_PIECES_CREATED[color][type])) return false;

	delete_piece(target);
	Piece* p = create_piece(type, this->color);
	place_piece(p, pos->x, pos->y);
	return true;
}


//////////// DELETION //////////////////////////////////////////////////////////////////////////

void delete_piece(Piece* p){
	p->remove();
	u8 size = pieces[p->color].size();
	u8 i = 0;
	for(; i < size; ++i) if(pieces[p->color][i] == p) break;
	pieces[p->color].erase(pieces[p->color].begin() + i);
	--piecesCreated[p->color][p->type];
	delete(p);
}


void delete_pieces(){
	for(u8 i = 0; i < NUM_COLORS; ++i){
		while(!pieces[i].empty()) delete_piece(pieces[i][0]);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////

void print_pieces(){
	for(u8 i = 0; i < NUM_COLORS; ++i){
		printf("Color %d Pieces:\n", i);
		for(Piece* piece: pieces[i]) printf("%d ", piece->type);
		printf("\n");
	}
}


