#include "picture.hpp"


Picture::Picture(){

    return;
}

Picture::Picture(SDL_Renderer* main_renderer, std::string name, int rect_x, int rect_y, int rect_w, int rect_h){

    renderer = main_renderer;
    if(renderer == NULL){
        std::cout << "nok\n";
        throw std::invalid_argument("Error: Recieved a null renderer.");
    }

    std::string path = "./images/" + name + ".png";
    char* path_str = new char [path.length()+1];
    strcpy(path_str, path.c_str());
    surface = IMG_Load(path_str);
    if(surface == NULL){
        std::cout << path_str;
        std::cout << "nok\n";
        throw std::invalid_argument("Error creating Picture: Ensure a valid PNG exists.");
    }
    texture = SDL_CreateTextureFromSurface(renderer,surface);
    if(texture == NULL){
        std::cout << "Error creating Picture texture";
        std::terminate();        
    }

    r.x = rect_x;
    r.y = rect_y;
    r.w = rect_w;
    r.h = rect_h;

    rect = &r;

    return;
}

// 

Picture::Picture(TTF_Font* font, SDL_Renderer* main_renderer, const char* text, int rect_x, int rect_y){

    renderer = main_renderer;
    if(renderer == NULL){
        std::cout << "nok\n";
        throw std::invalid_argument("Error: Recieved a null renderer.");
    }
    surface = TTF_RenderText_Blended(font, text, {0, 0, 0});
    if(surface == NULL){
        throw std::invalid_argument("Error creating Picture from font.");
    }
    texture = SDL_CreateTextureFromSurface(renderer,surface);
    if(texture == NULL){
        std::cout << "Error creating Picture texture";
        std::terminate();        
    }

    r.x = rect_x;
    r.y = rect_y;
    r.w = surface->w;
    r.h = surface->h;

    rect = &r;

    return;
}

void Picture::free_picture(){
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    return;
}

void Picture::render(){
    SDL_RenderCopy(renderer, texture, NULL, this->rect);
    return;
}