#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Hello World
// g++ main.cpp
// Compiling requires the SDL2 and SDL2_image libraries.
// SDL2_image can be installed on linux using the following command:
// sudo apt-get install libsdl2-image-dev

int main(){
    const int fps = 50;
    const int frame_delay = 1000 / fps;

    Uint64 frame_start_time = 0;
    int frame_duration = 0;

    int water = 5;
    std::cout << "Hello World" << water << "more junkk\n"; 

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Cool Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 10, 100, 255, 255);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);


    SDL_Event event;

    bool keep_running = true;
    bool left_pressed = false;
    bool right_pressed = false;
    bool left_pressed_prev = false;
    bool right_pressed_prev = false;

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



        frame_duration = SDL_GetTicks64() - frame_start_time;
        //std::cout << frame_duration << "\n";
        if(frame_delay > frame_duration){
        
            SDL_Delay(frame_delay - frame_duration);
        }
    }

    //SDL_Delay(5000);
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