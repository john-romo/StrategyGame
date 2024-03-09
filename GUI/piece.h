#include "picture.hpp"
#include "tile.hpp"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdexcept>
#include "button2.hpp"

class Piece2{
    private:
        Button2* button = nullptr;
        int type = 0;
        int current_x = 0;
        int current_y = 0;
    public:
        Piece2(SDL_Renderer* renderer, Tile* tile, int startx, int starty);
        Tile* tile = nullptr;
        void render();
        bool is_selected = false;
        int update_piece(bool left_pressed, bool right_pressed, int mouse_x, int mouse_y);

};