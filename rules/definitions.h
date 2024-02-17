// definitions.h

#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#define DEBUG 0

#include <cstdint>
#include <math.h>

#define u8 uint8_t
#define i8 int8_t
#define u16 uint16_t
#define i16 int16_t
#define ABS_MASK_8 0x7FFF

#define EAST (headings[0])
#define NORTHEAST (headings[1])
#define NORTH (headings[2])
#define NORTHWEST (headings[3])
#define WEST (headings[4])
#define SOUTHWEST (headings[5])
#define SOUTH (headings[6])
#define SOUTHEAST (headings[7])

#define KING 0
#define GUARD 1
#define RIFLEMAN 2
#define PARATROOPER 3
#define SPECOPS 4
#define ENGINEER 5
#define SCOUT 6
#define SEARCHLIGHT 7

#define WHITE 0
#define BLACK 1

#define ATTACK 0
#define DEFEND 1
#define STEALTH 2

#endif
