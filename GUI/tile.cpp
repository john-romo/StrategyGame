// This class is no longer in use and can be removed. (Replaced by Square)

#include "tile.hpp"

Tile::Tile(int x_location, int y_location, Picture* tpicture){

    x = x_location;
    y = y_location;
    picture = tpicture;

}

// Render the tile. Since it is placed on a scrollable map, the rectangle will need to be moved.

void Tile::render(int xmod, int ymod){
    picture->rect->x = xmod*32;
    picture->rect->y = ymod*32;
    SDL_RenderCopy(picture->renderer, picture->texture, NULL, picture->rect );
}
