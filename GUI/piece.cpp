#include "piece.h"

Piece2::Piece2(SDL_Renderer* renderer, Tile* the_tile, int startx, int starty){

    this->current_x = startx;
    this->current_y = starty;
    button = new Button2(renderer, "warning", startx*32, starty*32, 32, 32);
    this->tile = the_tile;

    return;
};

void Piece2::render(){
    return;
};

int Piece2::update_piece(bool left_pressed, bool right_pressed, int mouse_x, int mouse_y){
    if(this->button->was_clicked()){
        this->is_selected = true;
    }

    if(right_pressed){
        this->is_selected = false;
    }

    if(this->is_selected && left_pressed){
        this->current_x = floor(mouse_x/32);
        this->current_y = floor(mouse_y/32);
    }
    return 0;
}
