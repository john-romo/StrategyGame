// default.h

#ifndef _DEFAULT_H
#define _DEFAULT_H

#define DEBUG 1

#include <cstdint>
#include <math.h>
#include <iostream>


#define EAST (headings[0])
#define NORTHEAST (headings[1])
#define NORTH (headings[2])
#define NORTHWEST (headings[3])
#define WEST (headings[4])
#define SOUTHWEST (headings[5])
#define SOUTH (headings[6])
#define SOUTHEAST (headings[7])

#define NUM_PIECE_TYPES 8
#define IS_ATTACKING_PIECE(type) ((bool) ((type) > (3)))
enum PIECE_TYPES{
	KING,
	ENGINEER, 
	SCOUT,
	SEARCHLIGHT,
	GUARD,
	RIFLEMAN,
	SPECOPS,
	PARATROOPER
};


#define NUM_COLORS 2
enum COLORS{
	WHITE,
	BLACK
};


enum STANCES{
	ACTION,
	DEFEND,
	STEALTH
};

#define WIDTH 15
const int DIAG = (int) (WIDTH/sqrt(2));
const int HEIGHT = WIDTH + 2*DIAG - 2;
const int TOTAL = (WIDTH*WIDTH)+(4*(WIDTH*(DIAG-1)))+(4*(((DIAG-2)*(DIAG-1))/2));

#define MAX_STAMINA 10
#define STARTING_STAMINA 10
#define STAMINA_RECHARGE 3
 
#define WHITE_KING_MAX 1
#define WHITE_ENGINEER_MAX 2
#define WHITE_SCOUT_MAX 2
#define WHITE_SEARCHLIGHT_MAX 1
#define WHITE_GUARD_MAX 4
#define WHITE_RIFLEMAN_MAX 8
#define WHITE_SPECOPS_MAX 4
#define WHITE_PARATROOPER_MAX 4

#define BLACK_KING_MAX 1
#define BLACK_ENGINEER_MAX 2
#define BLACK_SCOUT_MAX 2
#define BLACK_SEARCHLIGHT_MAX 1
#define BLACK_GUARD_MAX 4
#define BLACK_RIFLEMAN_MAX 8
#define BLACK_SPECOPS_MAX 4
#define BLACK_PARATROOPER_MAX 4

const int MAX_PIECES_CREATED[2][NUM_PIECE_TYPES] =
{
	{
		WHITE_KING_MAX,
		WHITE_ENGINEER_MAX,
		WHITE_SCOUT_MAX,
		WHITE_SEARCHLIGHT_MAX,
		WHITE_GUARD_MAX,
		WHITE_RIFLEMAN_MAX,
		WHITE_SPECOPS_MAX, 
		WHITE_PARATROOPER_MAX

	},

	{
		BLACK_KING_MAX,
		BLACK_ENGINEER_MAX,
		BLACK_SCOUT_MAX,
		BLACK_SEARCHLIGHT_MAX,
		BLACK_GUARD_MAX,
		BLACK_RIFLEMAN_MAX,
		BLACK_SPECOPS_MAX,
		BLACK_PARATROOPER_MAX
	}
};

#define KING_STAMINA_DRAIN 1.0f
#define ENGINEER_STAMINA_DRAIN 2.0f
#define SCOUT_STAMINA_DRAIN 1.0f
#define SEARCHLIGHT_STAMINA_DRAIN -1.0f
#define GUARD_STAMINA_DRAIN 3.0f
#define RIFLEMAN_STAMINA_DRAIN 1.0f
#define SPECOPS_STAMINA_DRAIN 2.0f
#define PARATROOPER_STAMINA_DRAIN 2.0f

const float STAMINA_DRAIN[NUM_PIECE_TYPES] = 
{
	KING_STAMINA_DRAIN,
	ENGINEER_STAMINA_DRAIN,
	SCOUT_STAMINA_DRAIN,
	SEARCHLIGHT_STAMINA_DRAIN,
	GUARD_STAMINA_DRAIN,
	RIFLEMAN_STAMINA_DRAIN,
	SPECOPS_STAMINA_DRAIN,
	PARATROOPER_STAMINA_DRAIN
};

#define KING_HEALTH_DAMAGE 0
#define ENGINEER_HEALTH_DAMAGE 0
#define SCOUT_HEALTH_DAMAGE 0
#define SEARCHLIGHT_HEALTH_DAMAGE 0
#define GUARD_HEALTH_DAMAGE 0
#define RIFLEMAN_HEALTH_DAMAGE 2
#define SPECOPS_HEALTH_DAMAGE 0
#define PARATROOPER_HEALTH_DAMAGE 0

const int HEALTH_DAMAGE[NUM_PIECE_TYPES] = 
{
	KING_HEALTH_DAMAGE,
	ENGINEER_HEALTH_DAMAGE,
	SCOUT_HEALTH_DAMAGE,
	SEARCHLIGHT_HEALTH_DAMAGE,
	GUARD_HEALTH_DAMAGE,
	RIFLEMAN_HEALTH_DAMAGE,
	SPECOPS_HEALTH_DAMAGE,
	PARATROOPER_HEALTH_DAMAGE
};


#define KING_ARMOR_DAMAGE 0
#define ENGINEER_ARMOR_DAMAGE 0
#define SCOUT_ARMOR_DAMAGE 0
#define SEARCHLIGHT_ARMOR_DAMAGE 0
#define GUARD_ARMOR_DAMAGE 0
#define RIFLEMAN_ARMOR_DAMAGE 1
#define SPECOPS_ARMOR_DAMAGE 0
#define PARATROOPER_ARMOR_DAMAGE 0


const int ARMOR_DAMAGE[NUM_PIECE_TYPES] = 
{
	KING_ARMOR_DAMAGE,
	ENGINEER_ARMOR_DAMAGE,
	SCOUT_ARMOR_DAMAGE,
	SEARCHLIGHT_ARMOR_DAMAGE,
	GUARD_ARMOR_DAMAGE,
	RIFLEMAN_ARMOR_DAMAGE,
	SPECOPS_ARMOR_DAMAGE,
	PARATROOPER_ARMOR_DAMAGE
};

#define KING_STARTING_HEALTH 1
#define ENGINEER_STARTING_HEALTH 1
#define SCOUT_STARTING_HEALTH 1
#define SEARCHLIGHT_STARTING_HEALTH 1
#define GUARD_STARTING_HEALTH 1
#define RIFLEMAN_STARTING_HEALTH 4
#define SPECOPS_STARTING_HEALTH 1
#define PARATROOPER_STARTING_HEALTH 1

const int STARTING_HEALTH[NUM_PIECE_TYPES] =
{
	KING_STARTING_HEALTH,
	ENGINEER_STARTING_HEALTH,
	SCOUT_STARTING_HEALTH,
	SEARCHLIGHT_STARTING_HEALTH,
	GUARD_STARTING_HEALTH,
	RIFLEMAN_STARTING_HEALTH,
	SPECOPS_STARTING_HEALTH,
	PARATROOPER_STARTING_HEALTH
};

#define KING_STARTING_ARMOR 0
#define ENGINEER_STARTING_ARMOR 1
#define SCOUT_STARTING_ARMOR 1
#define SEARCHLIGHT_STARTING_ARMOR 1
#define GUARD_STARTING_ARMOR 2
#define RIFLEMAN_STARTING_ARMOR 1
#define SPECOPS_STARTING_ARMOR 1
#define PARATROOPER_STARTING_ARMOR 1

const int STARTING_ARMOR[NUM_PIECE_TYPES] =
{
	KING_STARTING_ARMOR,
	ENGINEER_STARTING_ARMOR,
	SCOUT_STARTING_ARMOR,
	SEARCHLIGHT_STARTING_ARMOR,
	GUARD_STARTING_ARMOR,
	RIFLEMAN_STARTING_ARMOR,
	SPECOPS_STARTING_ARMOR,
	PARATROOPER_STARTING_ARMOR
};

#define DEFEND_HEALTH_DAMAGE_MOD(hd) ((int) (hd) / (2))
#define DEFEND_ARMOR_DAMAGE_MOD(ad) ((int) (ad) / (2))
#define ZERO_ARMOR_DAMAGE_MOD(hd) ((int) (hd) + (1))


#define KING_VISION 5
#define ENGINEER_VISION 2
#define SCOUT_VISION 6
#define SEARCHLIGHT_VISION 5
#define GUARD_VISION 2
#define RIFLEMAN_VISION 4
#define SPECOPS_VISION 3
#define PARATROOPER_VISION 3

const int VISION[NUM_PIECE_TYPES] = 
{
	KING_VISION,
	ENGINEER_VISION,
	SCOUT_VISION,
	SEARCHLIGHT_VISION,
	GUARD_VISION,
	RIFLEMAN_VISION,
	SPECOPS_VISION,
	PARATROOPER_VISION
};

#endif