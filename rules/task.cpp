#include "task.h"

bool gameOver = false;
bool movementPhase = true;

Task::Task(int _t, Piece* _p, int _h){
	this->type = _t;
	this->piece = _p;
	this->headingID = _h;
}


void placement_phase(){
	
}

void movement_phase(){
	while(movementPhase){
		std::vector<Task*> tasks;
		
		for(int i = 0; i < NUM_COLORS; ++i){
			for(Piece* p : pieces[i]){
				p->mutex.lock();
				if(!p->tasks.empty()){
					Task* t = (Task*) p->tasks.front();
					p->tasks.pop();
					tasks.push_back(t);
				}
				p->mutex.unlock();
			}
		}

		auto rng = std::default_random_engine {};
		std::shuffle(std::begin(tasks), std::end(tasks), rng);

		std::stack<BinMove*> moves;

		while(!tasks.empty()){
			Task* t = tasks.back();
			tasks.pop_back();
			Piece* p = t->piece;
			if(t->type == MSG_TYPE_CANCEL) p->cancel(); 
			int x = p->x;
			int y = p->y;
			if(p->move(headings[t->headingID])){
				moves.push(create_binmove(x, y, p->x, p->y));	
			}
			delete(t);
		}

		//send_move_message(&moves);
	}
}















