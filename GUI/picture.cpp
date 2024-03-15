#include "picture.hpp"


// Picture::Picture(){

//     return;
// }

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
        throw std::invalid_argument("Error: Recieved a bad string.");
    }
    texture = SDL_CreateTextureFromSurface(renderer,surface);
    if(texture == NULL){
        
        std::cout << "Error creating texture";
        std::terminate();        
    }

    r.x = rect_x;
    r.y = rect_y;
    r.w = rect_w;
    r.h = rect_h;

    rect = &r;

    return;
}

void Picture::free_picture(){
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    return;
}

void Picture::render(){
    SDL_RenderCopy(renderer, texture, rect, NULL);
    return;
}