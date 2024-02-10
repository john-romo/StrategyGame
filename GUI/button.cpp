#include "button.hpp"

Button::Button(string bname){
    name = bname; 
    active = false;
    status = NORMAL;
    prev_status = NORMAL;
    return;
}

int Button::water_function(int stuff){
    return water;
}

void Button::update_button(bool left_pressed){
    prev_status = status;

    if(1){ // if mouse pos is at the button

        if(left_pressed){
            status = CLICKED;
            if(prev_status == HOVERED){
                active = true;
            }
        }else{
            status = HOVERED;
        }
    }else{
        status = NORMAL;
        active = false;
    }

    // -----------------

    if(status == prev_status){
        return;
    }else if(status == NORMAL){
        // swap picture
    }else if(status == HOVERED){
        // swamp picture
    }else{
        // swap picture
    }

    return;
}