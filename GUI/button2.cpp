#include "button2.hpp"
#include "picture.hpp"

//using namespace std;

// This is the class for clickable buttons.

// This constructor will set some default statuses and create Picture objects for each 
// possible button state: being clicked on, hovered over, or neither. 
// The bname argument indicates the name of the images that must be uploaded. 
// For example, entering "examplename" will attempt to upload three images:
// examplename_normal.png, examplename_hovered.png, and examplename_clicked.png.
// The images MUST be PNGs.

Button2::Button2(SDL_Renderer* renderer, std::string bname, int rect_x, int rect_y, int rect_w, int rect_h){

    Picture* normal = new Picture(renderer, bname+"_normal", rect_x, rect_y, rect_w, rect_h);
    Picture* hovered = new Picture(renderer, bname+"_hovered", rect_x, rect_y, rect_w, rect_h);
    Picture* clicked = new Picture(renderer, bname+"_clicked", rect_x, rect_y, rect_w, rect_h);
    this->type = bname;
    active = false;
    active_ptr = &active;
    status = NORMAL;
    prev_status = NORMAL;
    prev_status_ptr = &prev_status;
    status_ptr = &status;
    normal_picture = normal;
    hovered_picture = hovered;
    clicked_picture = clicked;
    current_picture = normal;
    return;
}

// Updates the current state of the button when needed and updates it, if necessary.
// it needs to know the mouse position (obtained from the event queue) and whether or 
// not the left mouse button was clicked this frame.
// This should always be called before was_clicked().

void Button2::update_button(bool left_pressed, SDL_Rect* mouse_pos){
    *prev_status_ptr = *status_ptr;
    
    if(SDL_HasIntersection(mouse_pos, current_picture->rect)){ // if mouse pos is at the button
        if(left_pressed){
            *status_ptr = CLICKED;
            this->current_picture = clicked_picture;
            if(this->prev_status == HOVERED){
                *active_ptr = true;
            }
        }else{
            *status_ptr = HOVERED;
            this->current_picture = hovered_picture;
        }
    }else{
        *status_ptr = NORMAL;
        this->current_picture = normal_picture;
        *active_ptr = false;
    }

    if(*status_ptr == *prev_status_ptr){
        return;
    }

    return;
}

// Check if a button was just clicked. A "click" is set to be
// when left was pressed and just now released over a button.
// update_button should always be called before this function.

bool Button2::was_clicked(void){
    if(*status_ptr == HOVERED && prev_status == CLICKED && active == true){
        *active_ptr = false;
        return true;
    }
    return false;
}

// This must be called to free the memory taken by the Pictures
// created by the Button2 constructor.

void Button2::free_button_pictures(){
    normal_picture->free_picture();
    hovered_picture->free_picture();
    clicked_picture->free_picture();
    delete normal_picture;
    delete hovered_picture;
    delete clicked_picture;
}

// render a button. The arguments are optional and can be ignored, 
// but can be useful for specific cases.

// xmod and ymod refer to a specific spot on the screen that the button should appear.
// This is useful for buttons which appear on the map. (these constantly change position)
// note: setting xmod or ymod will permanently change the position of the picture being rendered.

// setting use_default_picture to false can force a certain picture to display
// even if it would not otherwise. This is useful for piece selection
// (a "selected" piece appears as if it was clicked until deselected, even though its not "clicked" the entire time)
// to achieve this, current_picture should be set to the desired picture prior to calling render. 

void Button2::render(int xmod, int ymod, bool use_default_picture){
    // default case. render the picture where its at now.
    if(use_default_picture && xmod < 0 && ymod < 0){
        SDL_RenderCopy(current_picture->renderer, current_picture->texture, NULL, current_picture->rect );
    }
    else if(use_default_picture == false){
        if(xmod >= 0 && ymod >= 0){
            current_picture->rect->x = xmod*32;
            current_picture->rect->y = ymod*32;
        }
        SDL_RenderCopy(current_picture->renderer, current_picture->texture, NULL, current_picture->rect );
    }else{
        if(status == NORMAL){
            normal_picture->rect->x = xmod*32;
            normal_picture->rect->y = ymod*32;
        }else if(status == HOVERED){
            hovered_picture->rect->x = xmod*32;
            hovered_picture->rect->y = ymod*32;
        }else{
            clicked_picture->rect->x = xmod*32;
            clicked_picture->rect->y = ymod*32;
        }
        SDL_RenderCopy(current_picture->renderer, current_picture->texture, NULL, current_picture->rect );
        return;
    }
    return;

}
