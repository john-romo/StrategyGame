// player.h


#ifndef _PLAYER_H
#define _PLAYER_H

#include "default.h"
#include <cstdio>

class Player{
public:
	const bool color;
	i8 stamina;

	Player(u8 _color);

	bool drain_stamina(u8 rate);
	void recharge_stamina(u8 amount);
};

extern Player* playerWhite;
extern Player* playerBlack;
extern Player* players[NUM_PLAYERS];

void create_players();
void delete_players();

#endif
