#include "camera.hpp"

Camera::Camera(Tile* the_map[100][100]){
    return;
}

// This will actually draw the map, by rendering the Tile objects in view.

void Camera::display_map(){

    for(int x=current_x_pos; x<current_x_pos+20; x++){
        for(int y=current_y_pos; x<current_y_pos+15; y++){
            this->map[x][y]->render(x-current_x_pos,y-current_y_pos);
        }
    }
    return;
}

// Change the spot a player is looking at on the map, depending on where the mouse is.

void Camera::change_camera_pos(int mouse_x, int mouse_y){

    if(mouse_x > 630) current_x_pos += 1;
    else if(mouse_x < 10) current_x_pos -= 1;
    if(mouse_y > 470) current_y_pos += 1;
    else if(mouse_y < 10) current_y_pos -= 1;

    if(current_x_pos > 80) current_x_pos = 80;
    else if(current_x_pos < 0) current_x_pos = 0;
    if(current_y_pos > 85) current_y_pos = 85;
    else if(current_y_pos < 0) current_y_pos = 0;
    
    return;
}