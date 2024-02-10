#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "button.hpp"

using namespace std;

// Hello World
// g++ main.cpp
// Compiling requires the SDL2 and SDL2_image libraries.
// SDL2_image can be installed on linux using the following command:
// sudo apt-get install libsdl2-image-dev

enum menu {HOME, SETTINGS, SELECTION, GAME};
enum setting { ON, OFF };

int get_main_menu(SDL_Renderer* renderer, SDL_Texture* png, SDL_Rect* rect, menu* status, setting* fx, setting* music);
int init_png_support();

int main(){
    const int fps = 50;
    const int frame_delay = 1000 / fps;
    Uint64 frame_start_time = 0;
    int frame_duration = 0;
    int water = 5;
    cout << "Hello World\n";

    Button button("single player");
    Button button2("settings");
    Button button3("quit");
    Button button4("back");
    Button button5("play");
    Button left_arrow("<");
    Button right_arrow(">");

    
    int x = 0;
    x = button.water_function(2);
    menu menu_stat = HOME;
    menu* menu_status = &menu_stat;
    setting sound = OFF;
    setting* fx = &sound;
    setting mus = OFF;
    setting* music = &mus;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Cool Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    init_png_support();
    SDL_Event event;
    SDL_Surface* home_bg = IMG_Load("./images/bg.png");
    SDL_Texture* home_bg_texture = SDL_CreateTextureFromSurface(renderer,home_bg);
    SDL_Rect rect;
    rect.w = 640;
    rect.h = 480;
    rect.x = 0;
    rect.y = 0;
    SDL_Rect* rect_ptr = &rect;
    if(home_bg == NULL) return -1;
    

    bool keep_running = true;
    bool left_pressed = false;
    bool right_pressed = false;
    bool left_pressed_prev = false;
    bool right_pressed_prev = false;

    std::cout << "Hello World\n";

    while(keep_running){

        frame_start_time = SDL_GetTicks64();
        left_pressed_prev = left_pressed;
        right_pressed_prev = right_pressed;
        left_pressed = false;
        right_pressed = false;

       
        while(SDL_PollEvent(&event) != 0){ // a queue of events for this frame
            if(event.type == SDL_QUIT){
                keep_running = false;
                break;
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN){
                if(event.button.button == SDL_BUTTON_LEFT){
                    left_pressed = true;
                }else if(event.button.button == SDL_BUTTON_RIGHT){
                    right_pressed = true;
                }
            }
        }

        get_main_menu(renderer, home_bg_texture, rect_ptr, menu_status, fx, music);


        SDL_RenderPresent(renderer); // UPDATE THE SCREEN

        frame_duration = SDL_GetTicks64() - frame_start_time;
        //std::cout << frame_duration << "\n";
        if(frame_delay > frame_duration){
        
            SDL_Delay(frame_delay - frame_duration);
        }
    }

    //SDL_Delay(5000);
    SDL_FreeSurface(home_bg);
    SDL_DestroyTexture(home_bg_texture);
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}



int init_png_support(){
    if(IMG_Init(IMG_INIT_PNG) == 0){
        return -1;
    }
    return 0;
}

int get_main_menu(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* rect, menu* status, setting* fx, setting* music){
    if(*status == HOME){
        SDL_RenderCopy(renderer, texture, rect, NULL);
        // get home page and buttons

        //cout << "1\n";
    }else if (*status == SETTINGS){
        // get settings page and buttons
    }else if(*status == SELECTION){
        //cout << "3\n";
    }
    *fx = ON;
    
    if(*fx == ON ){
        //cout << "sound is on\n";
    }else{
        //cout << "its off\n";
    }
    return 0;
}