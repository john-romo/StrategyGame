// player.h


#ifndef _PLAYER_H
#define _PLAYER_H

#include "defaults.h"

class Player{
public:
	const bool color;
	bool stamina;

	Player(u8 _color);

	void drain_stamina(u8 rate);
	void recharge_stamina(u8 amount);
};

extern Player* playerWhite;
extern Player* playerBlack;

void create_players();
void delete_players();

#endif
