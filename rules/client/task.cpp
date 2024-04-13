#include "task.h"

bool gameOver = false;
std::vector<int*> movementMsgs;


Task::Task(int _t, Piece* _p, Heading _h){
	this->type = _t;
	this->piece = _p;
	this->heading = _h;
}


void movement(){
	std::vector<Task*> tasks;
		
	for(Piece* p : pieces[player->color]){
		if(!p->tasks.empty()){
			Task* t = (Task*) p->tasks.front();
			p->tasks.pop();
			tasks.push_back(t);
		}
	}

	auto rng = std::default_random_engine {};
	std::shuffle(std::begin(tasks), std::end(tasks), rng);

	while(!tasks.empty()){
		Task* t = tasks.back();
		tasks.pop_back();
		Piece* p = t->piece;
		Heading h = t->heading;
		if(t->type == CANCEL){
			while(!p->tasks.empty()){
				delete((Task*) p->tasks.front());
				p->tasks.pop();
			}
		}
		else if(t->type == MOVEMENT){
			if(!place_piece(p, p->x + h.x, p->y + h.y, false))
				p->bump(p->x + h.x, p->y + h.y);
		}
		delete(t);
	}
}








