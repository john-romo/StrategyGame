// main.cpp

#include <stdlib.h>

#include "main.h"
#include "board.h"
#include "player.h"
#include "test.h"
#include "default.h"

// includes for GUI
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <list>

#include "tile.hpp"
#include "button2.hpp"
#include "picture.hpp"
#include "camera.hpp"

enum menu {HOME, SETTINGS, SELECTION, GAME};
enum setting { ON, OFF };

void assign_tile_pointers(Tile* (*map)[100][100]);
int init_png_support();
int get_home_menu(menu* status, SDL_Renderer* renderer, SDL_Rect* mouse_pos, Picture bg, Button2 b1, Button2 b2, Button2 b3, bool* keep_running, bool clicked);
int get_settings_menu(menu* status, SDL_Renderer* renderer,  SDL_Rect* mouse_pos, Picture bg, Button2 b1, setting* fx, setting* music, bool clicked, Button2 on1, Button2 off1);
int get_selection_menu(menu* status, SDL_Renderer* renderer, Camera* camera, int mouse_x, int mouse_y);
Tile* create_tile(SDL_Renderer* renderer, int x, int y);
int create_map(SDL_Renderer* renderer);


int main(int, char**){
	

	// GUI and SDL stuff
	const int fps = 50;
    const int frame_delay = 1000 / fps;
    Uint64 frame_start_time = 0;
    int frame_duration = 0;
    menu menu_stat = HOME;
    menu* menu_status = &menu_stat;
    setting sound = OFF;
    setting* fx = &sound;
    setting mus = OFF;
    setting* music = &mus;

	SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Cool Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderPresent(renderer);
    init_png_support();

	SDL_Event event;

    Picture bg(renderer, "bg", 0, 0, 640, 480);
    Picture settings_bg(renderer, "settingsbg", 0, 0, 640, 480);
    Button2* b1 = new Button2(renderer, "single_player",100,430,132,30);
    Button2* b2 = new Button2(renderer, "settings",300,430,92,30);
    Button2* b3 = new Button2(renderer, "quit", 500,430,60,30);
    Button2* b4 = new Button2(renderer, "back", 300,430,60,30);
    Button2* b5 = new Button2(renderer, "play", 0,430,60,30);
    Button2* on1 = new Button2(renderer, "on", 500, 200, 60, 30);
    Button2* off1 = new Button2(renderer, "off", 500, 200, 60, 30);
    
    SDL_Rect mouse_rect;
    mouse_rect.x = 0;
    mouse_rect.y = 0;
    mouse_rect.w = 1;
    mouse_rect.h = 1;
    int mouse_x, mouse_y;
    
    bool keep_running = true;
    bool left_pressed = false;
    bool right_pressed = false;
    bool left_pressed_prev = false;
    bool right_pressed_prev = false;

    std::cout << "Hello World\n";

	// Tile *map[100][100];
    // for(int i=0; i<100; i++){
    //     for(int j=0; j<100; j++){
    //         map[i][j] = create_tile(renderer, i, j);
    //     }
    // }
    // assign_tile_pointers(&map);
    Camera test_camera;

	start_game(renderer);
 	//printf("%s\n\n", run_tests());
	//print_board();

	while(keep_running){
        //cout << mouse_x << endl;
        //cout << mouse_y << endl;

        frame_start_time = SDL_GetTicks64();
        left_pressed_prev = left_pressed;
        right_pressed_prev = right_pressed;
       
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
            }else if(event.type == SDL_MOUSEBUTTONUP){
                if(event.button.button == SDL_BUTTON_LEFT){
                    left_pressed = false;
                }else if(event.button.button == SDL_BUTTON_RIGHT){
                    right_pressed = false;
                }
            }else if(event.type == SDL_MOUSEMOTION){
                SDL_GetMouseState(&mouse_x, &mouse_y);
                mouse_rect.x = mouse_x;
                mouse_rect.y = mouse_y;
            }
        }

        // The following changes the current display depending on the state of the game

        if(*menu_status == HOME){
            get_home_menu(menu_status, renderer,&mouse_rect,bg,*b1,*b2,*b3,&keep_running,left_pressed);
        }else if(*menu_status == SETTINGS){
            get_settings_menu(menu_status, renderer, &mouse_rect, settings_bg, *b4, fx, music, left_pressed, *on1, *off1);
        }else if(*menu_status == SELECTION){
            get_selection_menu(menu_status, renderer, &test_camera, mouse_x, mouse_y);
        }else{
            ; // game is on
        }
        SDL_RenderPresent(renderer); // This will actually update what the player can see.

        frame_duration = SDL_GetTicks64() - frame_start_time;
        if(frame_delay > frame_duration){
            SDL_Delay(frame_delay - frame_duration);
        }
    }

    bg.free_picture();
    settings_bg.free_picture();
    b1->free_button_pictures();
    b2->free_button_pictures();
    b3->free_button_pictures();
    b4->free_button_pictures();
    b5->free_button_pictures();
    delete b1;
    delete b2;
    delete b3;
    delete b4;
    delete b5; 
    delete on1;
    delete off1;

    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();

	end_game();
	return 0;
}


void start_game(SDL_Renderer* renderer){
	printf("\n");
	create_players();
	create_headings();
	//create_board(renderer);
	create_board_filled(renderer);
	mark_valid_tiles(renderer);

}

void end_game(){
	delete_pieces();
	delete_board();
	delete_players();
	printf("\n");
}

int init_png_support(){
    if(IMG_Init(IMG_INIT_PNG) == 0){
        return -1;
    }
    return 0;
}



int get_home_menu(menu* status, SDL_Renderer* renderer, SDL_Rect* mouse_pos, Picture bg, Button2 b1, Button2 b2, Button2 b3, bool* keep_running, bool clicked){
    bg.render();
    b1.update_button(clicked, mouse_pos);
    b1.render();
    b2.update_button(clicked, mouse_pos);
    b2.render();
    b3.update_button(clicked, mouse_pos);
    b3.render();

    if(b1.was_clicked()){
        *status = SELECTION;
    }else if(b2.was_clicked()){
        *status = SETTINGS;
    }else if(b3.was_clicked()){
        *keep_running = false;
    }

    return 0;
}

int get_settings_menu(menu* status, SDL_Renderer* renderer, SDL_Rect* mouse_pos, Picture bg, Button2 b, setting* fx, setting* music, bool clicked, Button2 on1, Button2 off1){
    bg.render();
    b.update_button(clicked, mouse_pos);
    b.render();
    if(b.was_clicked()){
        *status = HOME;
    }
    if(*fx == ON){
        on1.update_button(clicked, mouse_pos);
        on1.render();
        if(on1.was_clicked()) *fx = OFF;
    }else{
        off1.update_button(clicked, mouse_pos);
        off1.render();
        if(off1.was_clicked()) *fx = ON;
    }
    return 0;
}

int get_selection_menu(menu* status, SDL_Renderer* renderer, Camera* camera, int mouse_x, int mouse_y){
    camera_display(camera->current_x_pos, camera->current_y_pos);
    camera->change_camera_pos(mouse_x, mouse_y);
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //SDL_RenderClear(renderer);
    return 0;
}
