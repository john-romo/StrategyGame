// task.h

#ifndef _TASK_H
#define _TASK_H

#include <queue>
#include <stack>
#include <vector>
#include <mutex>
#include <algorithm>
#include <chrono>
#include <random>

#include "../default.h"
#include "piece.h"
#include "board.h"
#include "heading.h"

extern bool gameOver;

class Task{
public:
	int type;
	Piece* piece;
	
	Heading heading;

	Task(int _t, Piece* _p, Heading _h);
};



void movement();

#endif
