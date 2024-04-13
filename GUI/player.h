// player.h


#ifndef _PLAYER_H
#define _PLAYER_H

#include "default.h"

class Player{
public:
	const int color;
	int stamina;

	Player(int _color);

	bool drain_stamina(int rate);
	void recharge_stamina(int amount);
};

extern Player* players[NUM_COLORS];
extern Player* player;

void create_players();
void delete_players();

#endif
