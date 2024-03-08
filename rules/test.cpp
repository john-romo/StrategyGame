// test.cpp


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

void test_player_init(){
	assert(players[WHITE]);
	assert(players[BLACK]);
	printf("test_player_init() passed\n");
}

////////////////////////////////////////////////////////////////////////////

void test_player_stamina(){
	assert(players[WHITE]->drain_stamina(1));
	assert(players[WHITE]->stamina == (STARTING_STAMINA-1));
	assert(!(players[WHITE]->drain_stamina(STARTING_STAMINA)));
	
	players[WHITE]->recharge_stamina(1);
	assert(players[WHITE]->stamina == STARTING_STAMINA);
	players[WHITE]->recharge_stamina(1);
	assert(players[WHITE]->stamina == STARTING_STAMINA);
	printf("test_player_stamina() passed\n");
}

////////////////////////////////////////////////////////////////////////////

void test_square_get(){
	Square* s;
	assert(!(s = get_square(DIAG-2, 0)));
	assert(!(s = get_square(DIAG-1, HEIGHT)));
	assert(s = get_square(DIAG-1, HEIGHT-1));
	printf("test_square_get() passed\n");
}

////////////////////////////////////////////////////////////////////////////

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

void test_piece_creation(){
	for(u8 c = 0; c < NUM_COLORS; ++c){
		for(u8 t = 0; t < NUM_PIECE_TYPES; ++t){
			u8 max = MAX_PIECES_CREATED[c][t];
			for(u8 i = 0; i < max; ++i){
				assert(create_piece(t,c));
			}
		}	
	}
	assert(!create_piece(KING, WHITE));
	assert(!create_piece(RIFLEMAN, BLACK));
	assert(!create_piece(NUM_PIECE_TYPES+1, WHITE));
	assert(piecesCreated[WHITE][KING] == MAX_PIECES_CREATED[WHITE][KING]);
	Piece* p = pieces[WHITE].back();
	delete_piece(pieces[WHITE].back());
	assert(piecesCreated[WHITE][PARATROOPER] == (MAX_PIECES_CREATED[WHITE][PARATROOPER]-1));
	assert(!create_piece(PARATROOPER, NUM_COLORS));
	delete_pieces();
	printf("test_piece_creation() passed\n");
}

////////////////////////////////////////////////////////////////////////////

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
	assert(players[WHITE]->stamina < STARTING_STAMINA);
	Piece* sl = create_piece(SEARCHLIGHT, BLACK);
	place_piece(sl, DIAG+4, 4);
	assert(!sl->move(DIAG+4, 5));
	assert(r->move(DIAG+5, 5));
	assert(r->move(DIAG+6, 6));
	delete_pieces();
	printf("test_piece_movement() passed\n");
}

////////////////////////////////////////////////////////////////////////////

void test_piece_target(){
	Piece* r = create_piece(RIFLEMAN, WHITE);
	place_piece(r, DIAG-1, 0);
	assert(!r->set_target(DIAG-1, 0));
	assert(!r->set_target(DIAG-1, 1));
	assert(!r->set_target(DIAG-1, HEIGHT-1));
	Piece* r2 = create_piece(RIFLEMAN, BLACK);
	place_piece(r2, DIAG-1, 1);
	assert(r->set_target(DIAG-1, 1));
	r->remove();
	place_piece(r, DIAG-1, 5);
	r->move(DIAG-1, 5);
	Piece* e = create_piece(ENGINEER, WHITE);
	place_piece(e, DIAG-2, 4);
	assert(e->set_target(DIAG-1, 5));
	r->move(DIAG-1, 3);
	assert(!r->set_target(DIAG-2, 4));
	delete_pieces();
	printf("test_piece_target() passed\n");
}

////////////////////////////////////////////////////////////////////////////

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
	rw->set_target(DIAG-1, 1);	
	assert(rw->attack());	
	assert(rb->armor == (RIFLEMAN_STARTING_ARMOR - RIFLEMAN_ARMOR_DAMAGE));
	assert(rb->health == (RIFLEMAN_STARTING_HEALTH - RIFLEMAN_HEALTH_DAMAGE));
	assert(rw->attack());
	assert(!rb->placed);
	Piece* rb2 = create_piece(RIFLEMAN, BLACK);
	place_piece(rb2, DIAG-1, 1);
	rb2->set_stance(DEFEND);
	rw->set_target(DIAG-1, 1);
	assert(rw->attack());
	assert(rb2->health == (RIFLEMAN_STARTING_HEALTH - DEFEND_HEALTH_DAMAGE_MOD(HEALTH_DAMAGE[rw->type])));
	assert(rb2->armor == (RIFLEMAN_STARTING_ARMOR - DEFEND_ARMOR_DAMAGE_MOD(ARMOR_DAMAGE[rw->type])));
	assert(rw->attack());
	delete_pieces();
	printf("test_piece_attack() passed\n");
}

////////////////////////////////////////////////////////////////////////////

void test_piece_repair(){
	Piece* rw = create_piece(RIFLEMAN, WHITE);
	place_piece(rw, DIAG-1, 0);
	Piece* rb = create_piece(RIFLEMAN, BLACK);
	place_piece(rb, DIAG-1, 1);
	rw->set_target(DIAG-1, 1);	
	rw->attack();	
	Piece* e = create_piece(ENGINEER, BLACK);
	place_piece(e, DIAG, 1);
	e->turn(&WEST);
	e->set_target(DIAG-1, 1);
	assert(e->repair());
	assert(rb->armor == STARTING_ARMOR[ENGINEER]);
	delete_pieces();
	printf("test_piece_repair() passed\n");
}

////////////////////////////////////////////////////////////////////////////

void test_piece_reassign(){
	Piece* k = create_piece(KING, WHITE);
	place_piece(k, DIAG, 0);
	Piece* rw = create_piece(RIFLEMAN, WHITE);
	place_piece(rw, DIAG-1, 0);
	Piece* rb = create_piece(RIFLEMAN, BLACK);
	place_piece(rb, DIAG+1, 0);
	assert(!k->reassign(ENGINEER));
	k->set_target(DIAG-1, 0);
	assert(!k->reassign(KING));
	assert(!k->reassign(SEARCHLIGHT));
	assert(!k->reassign(10));
	assert(k->reassign(ENGINEER));
	k->set_target(DIAG+1, 0);
	assert(!k->reassign(ENGINEER));
	delete_pieces();
	printf("test_piece_reassign() passed\n");
}












