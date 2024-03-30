// piece.cpp

#include "piece.h"
#include "board.h"
#include "task.h"


//////////// CREATION //////////////////////////////////////////////////////////////////////////

std::vector<Piece*> pieces[NUM_COLORS];
int piecesCreated[NUM_COLORS][NUM_PIECE_TYPES] = {0};
int ids = 0;
std::pair<int, int> startBounds[NUM_COLORS] = {std::make_pair(0, DIAG-1), std::make_pair(DIAG+WIDTH-2, HEIGHT-1)};


Piece* create_piece(int type, int color){
	if(color >= NUM_COLORS ) return NULL;
	if(type >= NUM_PIECE_TYPES) return NULL;
	if((piecesCreated[color][type] + 1) > MAX_PIECES_CREATED[type]) return NULL;
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


Piece::Piece(int _type, int _color) : type(_type), color(_color){
	this->id = ids++;
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
}


void create_pieces(){
	for(int i = 0; i < NUM_PIECE_TYPES; ++i){
		for(int j = 0; j < MAX_PIECES_CREATED[i]; ++j){
			Piece* p = create_piece(i, player->color);
		}
	}
}

bool place_piece(Piece* piece, int x, int y){
	if(piece->placed) return false;

	int bottom = startBounds[piece->color].first;
	int top = startBounds[piece->color].second;

	if(!((y >= bottom) && (y <= top))) return false;
	
	Square* s;
	if(!(s = get_square(x, y))) return false;
	if(s->occupied) return false;

	piece->remove();
	s->piece = piece;
	s->occupied = true;
	piece->x = x;
	piece->y = y;
	piece->square = s;
	piece->placed = true;
	piece->reveal();

	return true;
}

void default_placement(){
	
	std::vector<Piece*> ps = player->color ? pieces[BLACK] : pieces[WHITE];
	int bottom = player->color ? HEIGHT-1 : 0;
	int top = player->color ? HEIGHT-DIAG : DIAG-1;
	int ymod = player->color ? -1 : 1;
	
	//king
	place_piece(ps[0], CENTER-1, bottom+ymod);
	//engineer
	place_piece(ps[1], CENTER-2, top-(2*ymod));
	place_piece(ps[2], CENTER+2, top-(2*ymod));
	//scout
	place_piece(ps[3], CENTER-4, top-(2*ymod));
	place_piece(ps[4], CENTER+4, top-(2*ymod));
	//searchlight
	place_piece(ps[5], CENTER+1, bottom+ymod);
	//gaurd
	place_piece(ps[6], CENTER-4, bottom+(4*ymod));
	place_piece(ps[7], CENTER-2, bottom+(4*ymod));
	place_piece(ps[8], CENTER, bottom+4*ymod);
	place_piece(ps[9], CENTER+2, bottom+(4*ymod));
	//rifleman
	place_piece(ps[10], CENTER-7, top-ymod);
	place_piece(ps[11], CENTER-5, top-ymod);
	place_piece(ps[12], CENTER-3, top-ymod);
	place_piece(ps[13], CENTER-1, top-ymod);
	place_piece(ps[14], CENTER+1, top-ymod);
	place_piece(ps[15], CENTER+3, top-ymod);
	place_piece(ps[16], CENTER+5, top-ymod);
	place_piece(ps[17], CENTER+7, top-ymod);
	//specops
	place_piece(ps[18], CENTER-9, top-ymod);
	place_piece(ps[19], CENTER-11, top-ymod);
	place_piece(ps[20], CENTER+9, top-ymod);
	place_piece(ps[21], CENTER+11, top-ymod);
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

	int range = KING_VISION;

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

	for(int i = 0; i < GUARD_VISION; ++i){
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
	int color = this->color;
	std::vector<void*>* vect = &this->visibleSquares;
	
	line_reveal(square, heading, RIFLEMAN_VISION, color, vect);

	Square* select;
	if(select = move_selection(square, *heading.right, 1))
		select->reveal(color, vect);
	
	if(select = move_selection(square, *heading.left, 1))
		select->reveal(color, vect);
}


void Piece::specops_reveal(){
	if(this->stance == STEALTH) return;
	Square* square = (Square*) this->square;	
	bool color = this->color;
	Heading heading = this->heading;
	std::vector<void*>* vect = &this->visibleSquares;
	
	square->reveal(color, vect);
	
	Square* select = square;

	for(int i = 1; i < SPECOPS_VISION; ++i){
		if(select = move_selection(square, heading, i)) select->reveal(color, vect);
		if((i == SPECOPS_VISION - 2) && select){
			Square* lr;
			if(lr = move_selection(select, *heading.left, 1)) lr->reveal(color, vect);
			if(lr = move_selection(select, *heading.right, 1)) lr->reveal(color, vect);
		}
	}
}


void Piece::paratrooper_reveal(){
	if(this->stance == STEALTH) return;
};


void Piece::engineer_reveal(){
	Square* square = (Square*) this->square;	
	bool color = this->color;
	Heading heading = this->heading;
	std::vector<void*>* vect = &this->visibleSquares;
	
	Square* select = square;

	for(int i = 0; i < ENGINEER_VISION; ++i){
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
	for(int i = 0; i < NUM_COLORS; ++i){
		for(Piece* piece : pieces[i]) piece->reveal();
	}
}


//////////// MUTATIONS /////////////////////////////////////////////////////////////////////////

bool Piece::set_stance(int s){
	if(this->type == SEARCHLIGHT) return false;
	if(this->type == KING) return false;
	if(this->stance == s) return true;
	if((s == DEFEND) && (this->type == SCOUT)) return false;
	if((s == STEALTH) && !((this->type == SCOUT) || (this->type == SPECOPS) || (this->type == PARATROOPER))) return false;
	this->stance = s;
	this->unreveal();
	this->reveal();
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

bool Piece::set_target(int x, int y){
	if(this->type == SCOUT) return false;

	Square* ts = get_square(x, y);

	if(this->type == SEARCHLIGHT){
		int midline = (HEIGHT-1)/2;
		if(this->color == WHITE && y < midline) return true;
		if(this->color == BLACK && y > midline) return true;
		return false;
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



//////////// ACTIONS ///////////////////////////////////////////////////////////////////////////

void Piece::cancel(){
	while(!this->tasks.empty()){
		delete((Task*) this->tasks.front());
		this->tasks.pop();
	}
}

//--------------------------------------------------------------------------------------------//

bool Piece::move(Heading h){
	if(!this->placed) return false;
	if(this->type == SEARCHLIGHT) return false;
	if(this->stance != ACTION) return false;
	
	if(!players[this->color]->drain_stamina(STAMINA_DRAIN[this->type]))
		return false;

	place_piece(this, this->x + h.x, this->y + h.y);

	return true;
}


void Piece::bump(int x, int y){
	this->cancel();
	if(!get_square(x, y)) return;
	Piece* p = get_square(x, y)->piece;
	if(p->color != this->color){
		p->cancel();
		p->set_stance(ACTION);	
		this->set_stance(ACTION);
	}
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


//--------------------------------------------------------------------------------------------//

bool Piece::attack(){
	if(this->stance != ACTION) return false;
	if(!IS_ATTACKING_PIECE(this->type)) return false;
	if(!this->placed) return false; 
	if(!this->targetSquare) return false;
	Piece* target = ((Square*) this->targetSquare)->piece;
	int hd = HEALTH_DAMAGE[type];
	int ad = ARMOR_DAMAGE[type];
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
	if(this->type != ENGINEER) return false;
	if(!this->placed) return false;
	if(!this->targetSquare) return false;
	Piece* target = ((Square*) this->targetSquare)->piece;
	if(!target->armor < STARTING_ARMOR[target->type]) return false;
	++target->armor;
	return true;
}

//--------------------------------------------------------------------------------------------//

bool Piece::reassign(int type){
	if(this->type != KING) return false;
	if(this->stance != ACTION) return false;
	if(type >= NUM_PIECE_TYPES) return false;
	if((type == SEARCHLIGHT) || (type == KING)) return false;
	Square* pos = (Square*) this->targetSquare;
	if(!pos) return false;
	if(!pos->occupied) return false;
	Piece* target = pos->piece;
	if(target->type == type) return false;
	if(target->color != this->color) return false;

	if(!(piecesCreated[color][type] < MAX_PIECES_CREATED[type])) return false;

	delete_piece(target);
	Piece* p = create_piece(type, this->color);
	place_piece(p, pos->x, pos->y);
	return true;
}


//////////// DELETION //////////////////////////////////////////////////////////////////////////

void delete_piece(Piece* p){
	p->remove();
	int size = pieces[p->color].size();
	int i = 0;
	for(; i < size; i++) if(pieces[p->color][i] == p) break;
	pieces[p->color].erase(pieces[p->color].begin() + i);
	--piecesCreated[p->color][p->type];
	delete(p);
}


void delete_pieces(){
	for(int i = 0; i < NUM_COLORS; ++i){
		while(!pieces[i].empty()) delete_piece(pieces[i].front());
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////

void print_pieces(){
	for(int i = 0; i < NUM_COLORS; ++i){
		printf("Color %d Pieces:\n", i);
		for(Piece* piece: pieces[i]) printf("%d ", piece->type);
		printf("\n");
	}
}


