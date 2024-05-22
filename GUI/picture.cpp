#include "picture.hpp"

// Class to easily upload and render PNG images to a window. 
// It ONLY works with PNGs.

Picture::Picture(){
    return;
}

// This constructor takes a renderer pointer and a string name corresponding to the name of
// a PNG image, which it will attempt to upload from the images directory. 
// entering "example" for this string will attempt to upload example.png. The image must be a png.

// rect_x and rect_y are the pixel location that the image will go to on the game window.
// rect_w and rect_h are the width and height (in pixels) of the image.
// note: if rect_w and rect_h don't match the image dimensions,
// it will stretch/distort the image to fit the specified size.

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

// A second constructor, specifically for creating pictures containing text. 
// Enter in a font, the renderer pointer and the text for the Picture to contain.
// rect_x and rect_y are the same as the above constructor.
// the size will correspond to the font size specified when uploading the font.

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

// free the surface and texture contained in the picture.
// if memory was allocated for the picture, it still needs to be freed.

void Picture::free_picture(){
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    return;
}

// paste the picture

void Picture::render(){
    SDL_RenderCopy(renderer, texture, NULL, this->rect);
    return;
}