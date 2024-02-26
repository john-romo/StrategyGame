#include "button2.hpp"
#include "picture.hpp"

//using namespace std;

// This is the class for clickable buttons.

// This constructor will set some default statuses and create Picture objects for each 
// possible button state: being clicked on, hovered over, or neither. 

Button2::Button2(SDL_Renderer* renderer, Picture* normal, Picture* hovered, Picture* clicked){
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

// Checks the current state of the button when needed and 
// updates it, if necessary.

void Button2::update_button(bool left_pressed, SDL_Rect* mouse_pos){
    *prev_status_ptr = *status_ptr;
    
    if(SDL_HasIntersection(mouse_pos, current_picture->rect)){ // if mouse pos is at the button
        if(left_pressed){
            *status_ptr = CLICKED;
            if(this->prev_status == HOVERED){
                *active_ptr = true;
            }
        }else{
            *status_ptr = HOVERED;
        }
    }else{
        *status_ptr = NORMAL;
        *active_ptr = false;
    }

    if(*status_ptr == *prev_status_ptr){
        return;
    }
    if(*status_ptr == NORMAL){
        current_picture = normal_picture;
    }else if(*status_ptr == HOVERED){
        current_picture = hovered_picture;
    }else{
        current_picture = clicked_picture;
    }

    return;
}

void Button2::render(void){
    if(status == NORMAL){
        SDL_RenderCopy(normal_picture->renderer, normal_picture->texture, NULL, normal_picture->rect );
    }else if(status == HOVERED){
        SDL_RenderCopy(hovered_picture->renderer, hovered_picture->texture, NULL, hovered_picture->rect );
    }else{
        SDL_RenderCopy(clicked_picture->renderer, clicked_picture->texture, NULL, clicked_picture->rect );
    }
    return;
}

bool Button2::was_clicked(void){
    // if(*status_ptr  == HOVERED){
    //     std::cout << "1\n";
    // }
    // if(*status_ptr == CLICKED){
    //     std::cout << "2\n";
    // }
    // if(*status_ptr == HOVERED && prev_status == CLICKED ){
    //     std::cout << "3\n";
    // }
    if(*status_ptr == HOVERED && prev_status == CLICKED && active == true){
        //std::cout << "klik\n";
        return true;
    }
    return false;
}

void Button2::free_button_pictures(){
    delete normal_picture;
    delete hovered_picture;
    delete clicked_picture;
}

