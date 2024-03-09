#ifndef TILE_H
#define TILE_H

#include "picture.hpp"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdexcept>

// A class to represent each "tile" on the map.

class Tile{
    private:
        Picture* picture = nullptr;
        int type = 0;
        int piece = 0;
        std::string territory = "none";
        bool open = true;
        int x = 0;
        int y = 0;

    public:
        Tile(int x_location, int y_location, Picture* tpicture);
        int render(int xmod = 0, int ymod = 0);
        bool in_view(int n, int v);
        Tile* north = nullptr;
        Tile* south = nullptr;
        Tile* east = nullptr;
        Tile* west = nullptr;

};

#endif
