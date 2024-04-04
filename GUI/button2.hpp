#ifndef BUTTON_H
#define BUTTON_H

// Class to manage buttons on the main menu
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "picture.hpp"

//using namespace std;

enum Status { NORMAL, CLICKED, HOVERED };

class Button2{
    private:
        std::string name;
        
        bool* active_ptr;
        Status status; // current button status
        Status prev_status; //
        Status* status_ptr = nullptr; // current button status
        Status* prev_status_ptr = nullptr; //
        
        
    public:
        //Button2(SDL_Renderer* renderer, Picture* normal, Picture* hovered, Picture* clicked); // constructor
        Button2(SDL_Renderer* renderer, std::string bname, int rect_x, int rect_y, int rect_w, int rect_h);
        void update_button(bool pressed, SDL_Rect* mouse_pos);
        void render(int xmod = -1, int ymod = -1, bool default_picture = true);
        bool was_clicked();
        void free_button_pictures();
        bool active;
        std::string type;
        
        Picture* current_picture = nullptr;
        Picture* normal_picture = nullptr;
        Picture* clicked_picture = nullptr;
        Picture* hovered_picture = nullptr;
};

#endif
