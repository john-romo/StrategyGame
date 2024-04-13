// player.cpp


#include "player.h"

Player* players[NUM_COLORS];
Player* player;

Player::Player(int _color) : color(_color){
	this->stamina = STARTING_STAMINA;
}

bool Player::drain_stamina(int rate){
	if(DEBUG) return true;
	if(this->stamina < rate) return false;
	this->stamina -= rate;
	return true;
}

void Player::recharge_stamina(int amount){
	this->stamina += amount;
	if(this->stamina > MAX_STAMINA) this->stamina = MAX_STAMINA;
}

void create_players(){
	for(int i = 0; i < NUM_COLORS; ++i){
		players[i] = new Player(i);
	}
}

void delete_players(){
	for(Player* p : players) delete(p);
}
