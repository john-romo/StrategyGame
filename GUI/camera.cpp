#include "camera.hpp"
#include "default.h"

Camera::Camera(){
    //this->map = the_map;
    return;
}

// This will draw the map, by rendering each Tile object in view.
// Now removed and replaced with a separate function.

// void Camera::display_map(){
//     for(int x=current_x_pos; x<current_x_pos+20; x++){
//         for(int y=current_y_pos; y<current_y_pos+15; y++){
//             (*map)[x][y]->render(x-current_x_pos,y-current_y_pos);
//         }
//     }
//     return;
// }

// Change the spot a player is looking at on the map, depending on where the mouse is.
// this creates the "scrolling" effect.


void Camera::change_camera_pos(int mouse_x, int mouse_y){
    // if the x (pixel) position of the mouse is greater than 620, "scroll right" by one tile this frame.
    if(mouse_x > 620 && mouse_y < 640) current_x_pos += 1;
    else if(mouse_x < 20 && mouse_x >= 0) current_x_pos -= 1;
    if(mouse_y > 460 && mouse_y < 480) current_y_pos += 1;
    else if(mouse_y < 20 && mouse_y >= 0) current_y_pos -= 1;
    // The following checks will ensure the player can't scroll to an invalid position.
    // This confines a player to the area of the map.
    if(current_x_pos > HEIGHT - 20) current_x_pos = HEIGHT - 20;
    else if(current_x_pos < 0) current_x_pos = 0;
    if(current_y_pos > HEIGHT-15) current_y_pos = HEIGHT-15;
    else if(current_y_pos < 0) current_y_pos = 0;
    return;
}
