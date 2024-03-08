#include "test.h"


const char* run_tests(){
	test_player_init(); 
	test_player_stamina(); 
	test_square_get(); 
	test_square_attr();
	test_piece_creation(); 
	test_piece_placement(); 
	test_piece_movement(); 
	test_piece_target(); 
	test_piece_turn(); 
	test_piece_stance(); 
	test_piece_attack(); 
	test_piece_repair(); 
	test_piece_reassign(); 

	return "ALL TESTS PASSED\0";
}

////////////////////////////////////////////////////////////////////////////

// acceptance test 
void test_player_init(){
	assert(playerWhite);
	assert(playerBlack);
	printf("test_player_init() passed\n");
}

////////////////////////////////////////////////////////////////////////////

// acceptance test
void test_player_stamina(){
	assert(playerWhite->drain_stamina(1));
	assert(playerWhite->stamina == (STARTING_STAMINA-1));
	assert(!(playerWhite->drain_stamina(STARTING_STAMINA)));
	
	playerWhite->recharge_stamina(1);
	assert(playerWhite->stamina == STARTING_STAMINA);
	playerWhite->recharge_stamina(1);
	assert(playerWhite->stamina == STARTING_STAMINA);
	printf("test_player_stamina() passed\n");
}

////////////////////////////////////////////////////////////////////////////

// acceptance test
void test_square_get(){
	Square* s;
	assert(!(s = get_square(DIAG-2, 0)));
	assert(!(s = get_square(DIAG-1, HEIGHT)));
	assert(s = get_square(DIAG-1, HEIGHT-1));
	printf("test_square_get() passed\n");
}

////////////////////////////////////////////////////////////////////////////

// acceptance test
void test_square_attr(){
	Square* s;
	assert(s = get_square(DIAG-1, HEIGHT-1));
	assert(s->x == DIAG-1);
	assert(s->y == HEIGHT-1);
	assert(!s->visibleWhite);
	assert(!s->visibleBlack);
	printf("test_square_attr() passed\n");
}

////////////////////////////////////////////////////////////////////////////

// white box -> condition coverage
/*
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
*/
void test_piece_creation(){
	for(u8 c = 0; c < 2; ++c){
		for(u8 t = 0; t < NUM_PIECE_TYPES; ++t){
			u8 max;
			if(c == WHITE) max = maxWhitePiecesCreated[t];
			else max = maxBlackPiecesCreated[t];
			for(u8 i = 0; i < max; ++i){
				assert(create_piece(t,c));
			}
		}	
	}
	assert(!create_piece(KING, WHITE));
	assert(!create_piece(RIFLEMAN, BLACK));
	assert(!create_piece(10, WHITE));
	assert(whitePiecesCreated[KING] == maxWhitePiecesCreated[KING]);
	delete_piece(whitePieces.back());
	assert(whitePiecesCreated[SEARCHLIGHT] == (maxWhitePiecesCreated[SEARCHLIGHT]-1));
	assert(!create_piece(SEARCHLIGHT, 2));
	delete_pieces();
	printf("test_piece_creation() passed\n");
}

////////////////////////////////////////////////////////////////////////////

// integration -> Square & Piece classes -> big bang
void test_piece_placement(){
	Piece* k = create_piece(KING, BLACK);
	assert(!place_piece(k, 0, 0));
	assert(place_piece(k, DIAG-1, 0));
	assert((get_square(DIAG-1,0))->piece == k);
	assert(!place_piece(k, DIAG, HEIGHT-1));
	Piece* k2 = create_piece(GUARD, WHITE);
	assert(!place_piece(k2, DIAG-1, 0));
	delete_pieces();
	printf("test_piece_placement() passed\n");
}

////////////////////////////////////////////////////////////////////////////

// integration -> Piece & Square classes -> big bang
void test_piece_movement(){
	Piece* r = create_piece(RIFLEMAN, WHITE);
	assert(!r->move(DIAG+1, 1));
	place_piece(r, DIAG+1, 1);
	r->set_stance(DEFEND);
	assert(!r->move(DIAG+2, 2));
	r->set_stance(ACTION);
	assert(!r->move(0,0));
	assert(!r->move(DIAG+1, HEIGHT-1));
	assert(!r->move(DIAG+2, 3));
	assert(r->move(DIAG+2, 2));
	assert(playerWhite->stamina < STARTING_STAMINA);
	Piece* sl = create_piece(SEARCHLIGHT, BLACK);
	place_piece(sl, DIAG+4, 4);
	assert(!sl->move(DIAG+4, 5));
	assert(r->move(DIAG+5, 5));
	assert(r->move(DIAG+6, 6));
	delete_pieces();
	printf("test_piece_movement() passed\n");
}

////////////////////////////////////////////////////////////////////////////

// integration -> Piece & Square classes -> big bang
void test_piece_target(){
	Piece* r = create_piece(RIFLEMAN, WHITE);
	place_piece(r, DIAG-1, 0);
	assert(!r->target(DIAG-1, 0));
	assert(!r->target(DIAG-1, 1));
	assert(!r->target(DIAG-1, HEIGHT-1));
	Piece* r2 = create_piece(RIFLEMAN, BLACK);
	place_piece(r2, DIAG-1, 1);
	assert(r->target(DIAG-1, 1));
	r->remove();
	place_piece(r, DIAG-1, 5);
	r->move(DIAG-1, 5);
	Piece* e = create_piece(ENGINEER, WHITE);
	place_piece(e, DIAG-2, 4);
	assert(e->target(DIAG-1, 5));
	r->move(DIAG-1, 3);
	assert(!r->target(DIAG-2, 4));
	delete_pieces();
	printf("test_piece_target() passed\n");
}

////////////////////////////////////////////////////////////////////////////

// integration -> Piece, Square & Heading classes -> big bang
void test_piece_turn(){
	Piece* k = create_piece(KING, WHITE);
	place_piece(k, DIAG+1, 1);
	assert(!k->turn(&EAST));
	Piece* sl = create_piece(SEARCHLIGHT, WHITE);
 	place_piece(sl, DIAG+5, HEIGHT-5);
	assert(!sl->turn(&EAST));
	Piece* r = create_piece(RIFLEMAN, WHITE);	
	assert(!r->turn(&EAST));
	place_piece(r, DIAG-1, 5);
	assert(r->turn(&EAST));
	assert(r->heading.left == &NORTHEAST);
	assert(r->heading.reverse == &WEST);
	delete_pieces();
	printf("test_piece_turn() passed\n");
}

////////////////////////////////////////////////////////////////////////////

// white box -> condition coverage
/*
bool Piece::set_stance(u8 s){
	if(this->stance == s) return true;
	if(this->type == SEARCHLIGHT) return false;
	if(this->type == KING) return false;
	if((s == DEFEND) && (this->type == SCOUT)) return false;
	if((s == STEALTH) && !((this->type == SCOUT) || (this->type == SPECOPS) || (this->type == PARATROOPER))) return false;
	this->stance = s;
	return true;
}
*/
void test_piece_stance(){
	Piece* sl = create_piece(SEARCHLIGHT, WHITE);
	assert(!sl->set_stance(ACTION));
	Piece* k = create_piece(KING, WHITE);
	assert(!k->set_stance(DEFEND));
	Piece* s = create_piece(SCOUT, WHITE);
	assert(!s->set_stance(DEFEND));
	assert(s->set_stance(STEALTH));
	assert(s->set_stance(ACTION));
	Piece* so = create_piece(SPECOPS, WHITE);
	assert(so->set_stance(STEALTH));
	assert(so->set_stance(DEFEND));
	assert(so->set_stance(ACTION));
	Piece* r = create_piece(RIFLEMAN, WHITE);
	assert(!r->set_stance(STEALTH));
	assert(r->set_stance(DEFEND));
	delete_pieces();
	printf("test_piece_stance() passed\n");
}

////////////////////////////////////////////////////////////////////////////

// integration -> Piece & Square classes -> big bang 
void test_piece_attack(){
	Piece* k = create_piece(KING, WHITE);
	assert(!k->attack());
	Piece* e = create_piece(ENGINEER, WHITE);
	assert(!e->attack());
	Piece* s = create_piece(SCOUT, WHITE);
	assert(!s->attack());
	Piece* sl = create_piece(SEARCHLIGHT, WHITE);
	assert(!sl->attack());
	Piece* rw = create_piece(RIFLEMAN, WHITE);
	assert(!rw->attack());
	place_piece(rw, DIAG-1, 0);
	assert(!rw->attack());
	Piece* rb = create_piece(RIFLEMAN, BLACK);
	place_piece(rb, DIAG-1, 1);
	rw->target(DIAG-1, 1);	
	assert(rw->attack());	
	assert(rb->armor == (RIFLEMAN_STARTING_ARMOR - RIFLEMAN_ARMOR_DAMAGE));
	assert(rb->health == (RIFLEMAN_STARTING_HEALTH - RIFLEMAN_HEALTH_DAMAGE));
	assert(rw->attack());
	assert(rb->armor == 0);
	assert(rb->health = (RIFLEMAN_STARTING_HEALTH - (2*RIFLEMAN_HEALTH_DAMAGE) - 1));
	assert(!rb->placed);
	delete_pieces();
	printf("test_piece_attack() passed\n");
}

////////////////////////////////////////////////////////////////////////////

// integratoin -> Piece & Square classes -> big bang
void test_piece_repair(){
	Piece* rw = create_piece(RIFLEMAN, WHITE);
	place_piece(rw, DIAG-1, 0);
	Piece* rb = create_piece(RIFLEMAN, BLACK);
	place_piece(rb, DIAG-1, 1);
	rw->target(DIAG-1, 1);	
	rw->attack();	
	Piece* e = create_piece(ENGINEER, BLACK);
	place_piece(e, DIAG, 1);
	e->turn(&WEST);
	e->target(DIAG-1, 1);
	assert(e->repair());
	assert(rb->armor == startingArmor[ENGINEER]);
	delete_pieces();
	printf("test_piece_repair() passed\n");
}

////////////////////////////////////////////////////////////////////////////

// integrationg -> Piece & Square classes -> big bang
void test_piece_reassign(){
	Piece* k = create_piece(KING, WHITE);
	place_piece(k, DIAG, 0);
	Piece* rw = create_piece(RIFLEMAN, WHITE);
	place_piece(rw, DIAG-1, 0);
	Piece* rb = create_piece(RIFLEMAN, BLACK);
	place_piece(rb, DIAG+1, 0);
	assert(!k->reassign(ENGINEER));
	k->target(DIAG-1, 0);
	assert(!k->reassign(KING));
	assert(!k->reassign(SEARCHLIGHT));
	assert(!k->reassign(10));
	assert(k->reassign(ENGINEER));
	k->target(DIAG+1, 0);
	assert(!k->reassign(ENGINEER));
	delete_pieces();
	printf("test_piece_reassign() passed\n");
}












