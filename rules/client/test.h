// test.h


#ifndef _TEST_H
#define _TEST_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>

#include "../default.h"
#include "board.h"
#include "piece.h"
#include "heading.h"
#include "player.h"
#include "task.h"


void run_tests();
void test_player_init();
void test_player_stamina();
void test_square_get();
void test_square_attr();
void test_piece_creation();
void test_piece_placement();
void test_piece_movement();
void test_piece_target();
void test_piece_turn();
void test_piece_stance();
void test_piece_attack();
void test_piece_repair();
void test_piece_reassign();
void test_task_creation();

#endif
