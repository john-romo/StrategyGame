// piece.cpp

#include <algorithm>
#include "piece.h"
#include "board.h"

std::vector<Piece*> whitePieces;
std::vector<Piece*> blackPieces;
u8 whitePiecesCreated[8] = {0};
u8 blackPiecesCreated[8] = {0};


bool inc_created_pieces(u8 type, u8 color){
	if(color > BLACK) return false;
	if(color == WHITE){
		if((whitePiecesCreated[type]+1) > maxWhitePiecesCreated[type]) return false;
		++whitePiecesCreated[type];
	}
	else{
		if((blackPiecesCreated[type]+1) > maxBlackPiecesCreated[type]) return false;
		++blackPiecesCreated[type];
	}
	return true;
}

void dec_created_pieces(u8 type, u8 color){
	if(color == WHITE) --(whitePiecesCreated[type]);
	else --(blackPiecesCreated[type]);
}


Piece* create_piece(u8 type, u8 color){
	if(!inc_created_pieces(type, color)) return NULL;
	switch(type){
		case KING:
			return new Piece(KING, color, ACTION, KING_STAMINA_DRAIN, KING_HEALTH_DAMAGE, KING_ARMOR_DAMAGE, KING_STARTING_HEALTH, KING_STARTING_ARMOR);
		case GUARD:
			return new Piece(GUARD, color, ACTION, GUARD_STAMINA_DRAIN, GUARD_HEALTH_DAMAGE, GUARD_ARMOR_DAMAGE, GUARD_STARTING_HEALTH, GUARD_STARTING_ARMOR);
		case RIFLEMAN:
			return new Piece(RIFLEMAN, color, ACTION, RIFLEMAN_STAMINA_DRAIN, RIFLEMAN_HEALTH_DAMAGE, RIFLEMAN_ARMOR_DAMAGE, RIFLEMAN_STARTING_HEALTH, RIFLEMAN_STARTING_ARMOR);
		case SPECOPS:
			return new Piece(SPECOPS, color, ACTION, SPECOPS_STAMINA_DRAIN, SPECOPS_HEALTH_DAMAGE, SPECOPS_ARMOR_DAMAGE, SPECOPS_STARTING_HEALTH, SPECOPS_STARTING_ARMOR);
		case PARATROOPER:
			return new Piece(PARATROOPER, color, ACTION, PARATROOPER_STAMINA_DRAIN, PARATROOPER_HEALTH_DAMAGE, PARATROOPER_ARMOR_DAMAGE, PARATROOPER_STARTING_HEALTH, PARATROOPER_STARTING_ARMOR);
		case ENGINEER:
			return new Piece(ENGINEER, color, ACTION, ENGINEER_STAMINA_DRAIN, ENGINEER_HEALTH_DAMAGE, ENGINEER_ARMOR_DAMAGE, ENGINEER_STARTING_HEALTH, ENGINEER_STARTING_ARMOR);
		case SCOUT:
			return new Piece(SCOUT, color, ACTION, SCOUT_STAMINA_DRAIN, SCOUT_HEALTH_DAMAGE, SCOUT_ARMOR_DAMAGE, SCOUT_STARTING_HEALTH, SCOUT_STARTING_ARMOR);
		case SEARCHLIGHT:
			return new Piece(SEARCHLIGHT, color, DEFEND, SEARCHLIGHT_STAMINA_DRAIN, SEARCHLIGHT_HEALTH_DAMAGE, SEARCHLIGHT_ARMOR_DAMAGE, SEARCHLIGHT_STARTING_HEALTH, SEARCHLIGHT_STARTING_ARMOR);
		default:
			return NULL;
	}
}


Piece::Piece(u8 _type, u8 _color, u8 _stance, float _staminaDrain, u8 _healthDamage, u8 _armorDamage, i8 _health, i8 _armor){
	this->type = _type;
	this->color = _color;
	this->stance = _stance;
	this->armor = _armor;
	this->health = _health;
	this->staminaDrain = _staminaDrain;
	this->healthDamage = _healthDamage;
	this->armorDamage = _armorDamage;
	this->placed = false;
	this->x = 0;
	this->y = 0;
	if(!this->color){
		this->player = playerWhite;
		this->heading = NORTH;
	}
	else{
		this->player = playerBlack;
		this->heading = SOUTH;
	}
	this->square = NULL;
	this->targetSquare = NULL;
	push_piece(this);
}




////////////////////////////////////////////////////////////////////////////////////////////////

void Piece::reveal(){
	switch(this->type){
		case KING:
			king_reveal(this);
			break;
		case GUARD:
			guard_reveal(this);
			break;
		case RIFLEMAN:
			rifleman_reveal(this);
			break;
		case SPECOPS:
			specops_reveal(this);
			break;
		case PARATROOPER:
			paratrooper_reveal(this);
			break;
		case ENGINEER:
			engineer_reveal(this);
			break;
		case SCOUT:
			scout_reveal(this);
			break;
		case SEARCHLIGHT:
			searchlight_reveal(this);
			break;
		default:
			break;
	}
}


void king_reveal(Piece* p){
	Square* square = (Square*) p->square;
	bool color = p->color;
	Heading heading = p->heading;
	std::vector<void*>* vect = &p->visibleSquares;
	
	u8 range = KING_VISION;

	Square* start;
	if(!(start = move_selection(square, *heading.reverse, 1))){
		start = square;
		--range;
	}

	Heading direction;
	if(p->x < HEIGHT/2) direction = EAST;
	else direction = WEST;
	
	start = move_selection(start, direction, KING_VISION/2);

	for(int i = 0; i < range; ++i){
		line_reveal(start, *direction.reverse, KING_VISION, color, vect);
		if(!(start = move_selection(start, heading, 1))) break;
	}
}


void guard_reveal(Piece* p){
	Square* square = (Square*) p->square;	
	bool color = p->color;
	Heading heading = p->heading;
	std::vector<void*>* vect = &p->visibleSquares;
	
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


void rifleman_reveal(Piece* p){
	Square* square = (Square*) p->square;
	Heading heading = p->heading;
	u8 color = p->color;
	std::vector<void*>* vect = &p->visibleSquares;
	
	line_reveal(square, heading, RIFLEMAN_VISION, color, vect);

	Square* select;
	if(select = move_selection(square, *heading.right, 1))
		select->reveal(color, vect);
	
	if(select = move_selection(square, *heading.left, 1))
		select->reveal(color, vect);
}


void specops_reveal(Piece* p){
	Square* square = (Square*) p->square;	
	bool color = p->color;
	Heading heading = p->heading;
	std::vector<void*>* vect = &p->visibleSquares;
	
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


void paratrooper_reveal(Piece* p){};


void engineer_reveal(Piece* p){
	Square* square = (Square*) p->square;	
	bool color = p->color;
	Heading heading = p->heading;
	std::vector<void*>* vect = &p->visibleSquares;
	
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


void scout_reveal(Piece* p){
	Square* square = (Square*) p->square;
	bool color = p->color;
	Heading heading = p->heading;
	std::vector<void*>* vect = &p->visibleSquares;

	if(p->stance != STEALTH){
		line_reveal(square, heading, SCOUT_VISION, color, vect);

		Heading lr[2] = {*p->heading.left, *p->heading.right};
		for(int i = 0; i < 2; ++i){
			for(int j = 0, r = SCOUT_VISION-1; j < SCOUT_VISION; ++j, --r){
				if(!(square = move_selection(square, lr[i], 1))) break;
				line_reveal(square, heading, r, color, vect);
			}
			square = (Square*) p->square;
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


void searchlight_reveal(Piece* p){
	Square* square = (Square*) p->targetSquare;
	if(!square) return;
	bool color = p->color;
	std::vector<void*>* vect = &p->visibleSquares;
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


void reveal_pieces(){
	for(Piece* piece: whitePieces) piece->reveal();
	for(Piece* piece: blackPieces) piece->reveal();
}


void Piece::unreveal(){
	for(void* square : this->visibleSquares){
		square_unreveal(square, this->color);
	}
	this->visibleSquares.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool Piece::set_stance(u8 s){
	if(this->stance == s) return true;
	if(this->type == SEARCHLIGHT) return false;
	if(this->type == KING) return false;
	if((s == DEFEND) && (this->type == SCOUT)) return false;
	if((s == STEALTH) && !((this->type == SCOUT) || (this->type == SPECOPS) || (this->type == PARATROOPER))) return false;
	this->stance = s;
	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////

bool Piece::turn(Heading* h){
	if(!this->placed) return false;
	if((this->type == KING) || (this->type == SEARCHLIGHT)) return false;
	this->heading = *h;
	this->unreveal();
	this->reveal();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool Piece::target(u8 x, u8 y){
	if(this->type > NUM_PIECE_TYPES-1) return false;
	if(this->type == SCOUT) return false;

	Square* ts = get_square(x, y);

	if(this->type == SEARCHLIGHT){
		bool check = searchlight_target(this, y);
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

bool searchlight_target(Piece* p, u8 y){
	u8 midline = (HEIGHT-1)/2;
	if(p->color == WHITE && y < midline) return true;
	if(p->color == BLACK && y > midline) return true;
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool Piece::attack(){
	if((this->type < GUARD) || (this->type > SPECOPS)) return false;
	if(!this->placed) return false; 
	if(!this->targetSquare) return false;
	Piece* target = ((Square*) this->targetSquare)->piece;
	u8 hd = this->healthDamage;
	u8 ad = this->armorDamage;
	if(target->stance == DEFEND){
		hd = hd/2;
		ad = ad/2;
	}
	target->armor -= ad;
	if(target->armor < 0){
		target->armor = 0;
		hd = hd+1;
	}
	target->health -= hd;
	if(target->health < 0) delete_piece(target);
	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////

bool Piece::repair(){
	if(!this->type == ENGINEER) return false;
	if(!this->placed) return false;
	if(!this->targetSquare) return false;
	Piece* target = ((Square*) this->targetSquare)->piece;
	if(!target->armor < startingArmor[target->type]) return false;
	++target->armor;
	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////

bool Piece::reassign(u8 type){
	if(type > NUM_PIECE_TYPES-1) return false;
	if((type == SEARCHLIGHT) || (type == KING)) return false;
	Square* pos = (Square*) this->targetSquare;
	if(!pos) return false;
	if(!pos->occupied) return false;
	Piece* target = pos->piece;
	if(target->type == type) return false;
	if(target->color != this->color) return false;

	if(this->color == WHITE){
		if(!(whitePiecesCreated[type] < maxWhitePiecesCreated[type])) return false;
	}
	else if(!(blackPiecesCreated[type] < maxBlackPiecesCreated[type])) return false;
	delete_piece(target);
	Piece* p = create_piece(type, this->color);
	place_piece(p, pos->x, pos->y);
	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////

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


bool Piece::move(u8 x, u8 y){
	if(!this->placed) return false;
	if(this->type == SEARCHLIGHT) return false;
	if(this->stance == DEFEND) return false;
	Square* s = get_square(x, y);
	if(!s) return false;
	if(!this->player->drain_stamina(this->staminaDrain * this->get_distance(x, y)))
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


////////////////////////////////////////////////////////////////////////////////////////////////

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
	piece->reveal();

	return true;
}


void push_piece(Piece* piece){
	if(!piece->color) whitePieces.push_back(piece);
	else blackPieces.push_back(piece);
}


void print_pieces(){
	printf("WHITE PIECES: ");
	for(Piece* piece: whitePieces) printf("%d ", piece->type);
	printf("\n");
	printf("BLACK PIECES: ");
	for(Piece* piece: blackPieces) printf("%d ", piece->type);
	printf("\n");


}

void delete_piece(Piece* p){
	p->remove();
	if(p->color == WHITE){
		u8 size = whitePieces.size();
		u8 i = 0;
		for(; i < size; ++i) if(whitePieces[i] == p) break;
		whitePieces.erase(whitePieces.begin() + i);
	}
	else{
		u8 size = blackPieces.size();
		u8 i = 0;
		for(; i < size; ++i) if(blackPieces[i] == p) break;
		blackPieces.erase(blackPieces.begin() + i);
	}
	dec_created_pieces(p->type, p->color);
	delete(p);
}

void delete_pieces(){
	while(!whitePieces.empty()){
		delete_piece(whitePieces[0]);
	}
	while(!blackPieces.empty()){
		delete_piece(blackPieces[0]);
	}
}

