#ifndef PICTURE_H
#define PICTURE_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <stdexcept>

//using namespace std;

// This is a class to set up and manage everything needed to 
// show a picture on the screen with SDL. This is inteded to make it easier 
// to create and pass things around. 

class Picture{
    private:
    ;
    public:
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect r;
    SDL_Rect* rect;


    Picture(SDL_Renderer* main_renderer, std::string name, int rect_x, int rect_y, int rect_w, int rect_h);
    Picture(TTF_Font* font, SDL_Renderer* main_renderer, const char* name, int rect_x, int rect_y, int rect_w, int rect_h);
    //Picture(SDL_Renderer* main_renderer, std::string name, int rect_x, int rect_y, int rect_w, int rect_h);
    //Picture();
    void free_picture();
    void render();

};

#endif
