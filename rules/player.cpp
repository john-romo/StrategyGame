// player.cpp


#include "player.h"

Player* playerWhite;
Player* playerBlack;

Player::Player(u8 _color) : color(_color){
	this->stamina = STARTING_STAMINA;
}

bool Player::drain_stamina(u8 rate){
	if(this->stamina < rate) return false;
	this->stamina -= rate;
	return true;

	if(this->stamina > temp) this->stamina = 0; 
}

void Player::recharge_stamina(u8 amount){
	this->stamina += amount;
	if(this->stamina > MAX_STAMINA) this->stamina = MAX_STAMINA;
}

void create_players(){
	playerWhite = new Player(WHITE);
	playerBlack = new Player(BLACK);
}

void delete_players(){
	delete(playerWhite);
	delete(playerBlack);
}
