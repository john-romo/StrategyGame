// player.cpp


#include "player.h"

Player* players[NUM_PLAYERS];

Player::Player(u8 _color) : color(_color){
	this->stamina = STARTING_STAMINA;
}

bool Player::drain_stamina(u8 rate){
	if(this->stamina < rate) return false;
	this->stamina -= rate;
	return true;
}

void Player::recharge_stamina(u8 amount){
	this->stamina += amount;
	if(this->stamina > MAX_STAMINA) this->stamina = MAX_STAMINA;
}

void create_players(){
	for(u8 i = 0; i < NUM_PLAYERS; ++i){
		players[i] = new Player(i);
	}
}

void delete_players(){
	for(Player* p : players) delete(p);
}
