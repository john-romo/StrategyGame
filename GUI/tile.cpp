#include "tile.hpp"

Tile::Tile(int x_location, int y_location, Picture* tpicture){

    x = x_location;
    y = y_location;
    picture = tpicture;

}

// Render the tile. Since it is placed on a scrollable map, the rectangle will need to be moved.

void Tile::render(int xmod = 0, int ymod = 0){
    SDL_RenderCopy(picture->renderer, picture->texture, NULL, picture->rect );
}
