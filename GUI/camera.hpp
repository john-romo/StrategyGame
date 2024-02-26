#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "picture.hpp"
#include "tile.hpp"

// Class to manage the display for each player.

class Camera{
    private:

        int num_x_tiles = 20;
        int num_y_tiles = 15;
        int current_x_pos = 0;
        int current_y_pos = 0;
    
    public:

        Tile* map[100][100];
        Camera(Tile* the_map[100][100]);
        void display_map();
        void change_camera_pos(int mouse_x, int mouse_y);

};

#endif
