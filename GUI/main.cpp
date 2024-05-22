// main.cpp
// new library for fonts required: sudo apt-get install libsdl2-ttf-dev

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
#include <SDL2/SDL_ttf.h>
#include <list>

#include "button2.hpp"
#include "picture.hpp"
#include "camera.hpp"
#include "piece.h"
#include "client.h"

enum menu {HOME, SETTINGS, WAITING, GAME};
enum game_status {NOT_ON, PLACEMENT, MOVEMENT_PHASE};
enum setting { ON, OFF };

//void assign_tile_pointers(Tile* (*map)[100][100]);
int init_png_and_font();
int get_home_menu(menu* status, SDL_Renderer* renderer, SDL_Rect* mouse_pos, Picture bg, Button2 b1, Button2 b2, Button2 b3, bool* keep_running, bool clicked);
int get_settings_menu(menu* status, SDL_Renderer* renderer,  SDL_Rect* mouse_pos, Picture bg, Button2 b1, setting* fx, setting* music, bool clicked, Button2 on1, Button2 off1);
int get_selection_menu(menu* status, SDL_Renderer* renderer, Camera* camera, int mouse_x, int mouse_y);
int create_map(SDL_Renderer* renderer);

int main(int, char**){
	
	// SDL stuff
	const int fps = 50; // this controls the frame rate
    const int frame_delay = 1000 / fps;
    int color = 0;
    Uint64 frame_start_time = 0;
    int frame_duration = 0;
    menu menu_stat = HOME;
    menu* menu_status = &menu_stat;
    setting sound = OFF;
    setting* fx = &sound;
    setting mus = OFF;
    setting* music = &mus;
    game_status the_game_status = NOT_ON;
    // Setup SDL
	SDL_Init(SDL_INIT_EVERYTHING);
    int screen_width = 640;
    int screen_height = 480;
    SDL_Window* window = SDL_CreateWindow("Cool Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderPresent(renderer);
    init_png_and_font();
    
    // Uploading the font in three different sizes
    
    TTF_Font* fontbig = TTF_OpenFont("Ubuntu-B.ttf", 32);
    TTF_Font* font = TTF_OpenFont("Ubuntu-B.ttf", 20);
    TTF_Font* fontsmall = TTF_OpenFont("Ubuntu-B.ttf", 16);
    
    // Making arrays of Pictures containing text to render to the screen
    // (explanation is in the picture class)
    Picture* hpstatus[10];
    hpstatus[0] = new Picture (font, renderer, "0", 560, 80);
    hpstatus[1] = new Picture (font, renderer, "1", 560, 80);
    hpstatus[2] = new Picture (font, renderer, "2", 560, 80);
    hpstatus[3] = new Picture (font, renderer, "3", 560, 80);
    hpstatus[4] = new Picture (font, renderer, "4", 560, 80);
    hpstatus[5] = new Picture (font, renderer, "5", 560, 80);
    hpstatus[6] = new Picture (font, renderer, "6", 560, 80);
    hpstatus[7] = new Picture (font, renderer, "7", 560, 80);
    hpstatus[8] = new Picture (font, renderer, "8", 560, 80);
    hpstatus[9] = new Picture (font, renderer, "9", 560, 80);

    Picture* armorstatus[5];
    armorstatus[0] = new Picture (font, renderer, "0", 560, 125);
    armorstatus[1] = new Picture (font, renderer, "1", 560, 125);
    armorstatus[2] = new Picture (font, renderer, "2", 560, 125);
    armorstatus[3] = new Picture (font, renderer, "3", 560, 125);
    armorstatus[4] = new Picture (font, renderer, "4", 560, 125);

    Picture* maxhp[5];
    maxhp[0] = new Picture (font, renderer, "0", 586, 80);
    maxhp[1] = new Picture (font, renderer, "1", 586, 80);
    maxhp[2] = new Picture (font, renderer, "2", 586, 80);
    maxhp[3] = new Picture (font, renderer, "3", 586, 80);
    maxhp[4] = new Picture (font, renderer, "4", 586, 80);

    Picture* unit_type[8];
    unit_type[0] = new Picture(font, renderer, "King", 524, 50);
    unit_type[1] = new Picture(font, renderer, "Engineer", 524, 50);
    unit_type[2] = new Picture(font, renderer, "Scout", 524, 50);
    unit_type[3] = new Picture(font, renderer, "Searchlight", 524, 50);
    unit_type[4] = new Picture(font, renderer, "Guard", 524, 50);
    unit_type[5] = new Picture(font, renderer, "Rifleman", 524, 50);
    unit_type[6] = new Picture(font, renderer, "Specops", 524, 50);
    unit_type[7] = new Picture(font, renderer, "Paratrooper", 524, 50);
    
    // Single PNG pictures and Buttons

    Picture slash(font, renderer, "/", 574, 80);
    Picture bg(renderer, "bg", 0, 0, 640, 480);
    bg.render();
    SDL_RenderPresent(renderer);

    Picture* loading = new Picture(fontbig, renderer, "Waiting for opponent to join...", 100, 100);
    Picture settings_bg(renderer, "settingsbg", 0, 0, 640, 480);
    Picture sidebar(renderer, "settingsbg", 512, 0, 128, 480);
    Picture sidebar2(renderer, "settingsbg", 0, 416, 640, 64);
    Picture hp_img(renderer, "heart", 525, 80, 32, 29);
    Picture armor_img(renderer, "armor", 525, 120, 32, 32);
    Picture black_img(renderer, "blackflag", 200, 430, 32, 32);
    Picture white_img(renderer, "whiteflag", 200, 430, 32, 32);
    Picture sidebar_black(renderer, "blackflag", 525, 18, 32, 32);
    Picture sidebar_white(renderer, "whiteflag", 525, 18, 32, 32);
    Picture* your_team = new Picture(fontbig, renderer, "Your team:", 20, 430);
    Picture* team_image;
    Picture* blank = new Picture(renderer, "grass4dark", 0, 0, 32, 32);
    
    Picture select_ctrl(renderer, "selectctrl", 100, 428, 440, 52); // 100, 426
    Button2* b1 = new Button2(renderer, "joingame",100,430,107,30);
    Button2* b2 = new Button2(renderer, "settings",300,430,92,30);
    Button2* b3 = new Button2(renderer, "quit", 500,430,60,30);
    Button2* b4 = new Button2(renderer, "back", 300,430,60,30);
    Button2* b5 = new Button2(renderer, "play", 0,430,60,30);
    Button2* on1 = new Button2(renderer, "on", 500, 200, 60, 30);
    Button2* off1 = new Button2(renderer, "off", 500, 200, 60, 30);
    Button2* placement_ready = new Button2(renderer, "play", 550, 440, 60, 30);
    Button2* end_turn = new Button2(renderer, "end_turn", 535, 435, 94, 30);

    Button2* placement_buttons[8];

    placement_buttons[0] = new Button2(renderer, "king", 160, 450, 32, 32 );
    placement_buttons[1] = new Button2(renderer, "engineer", 200, 450, 32, 32 );
    placement_buttons[2] = new Button2(renderer, "scout", 240, 450, 32, 32 );
    placement_buttons[3] = new Button2(renderer, "searchlight", 280, 450, 32, 32 );
    placement_buttons[4] = new Button2(renderer, "guard", 320, 450, 32, 32 );
    placement_buttons[5] = new Button2(renderer, "rifleman", 360, 450, 32, 32 );
    placement_buttons[6] = new Button2(renderer, "specops", 400, 450, 32, 32 );
    placement_buttons[7] = new Button2(renderer, "paratrooper", 440, 450, 32, 32 );

    Button2* current_selected_button = nullptr;
    Piece* current_selected_piece = nullptr;

    // other SDL stuff and variables used by the main loop

    SDL_Event event;
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

    Camera test_camera;

    std::cout << "Hello World\n";

    // sets up game rules

 	//printf("%s\n\n", run_tests());
	//print_board();

	while(keep_running){ // This is the main loop for the game.
        //cout << mouse_x << endl;
        //cout << mouse_y << endl;

        frame_start_time = SDL_GetTicks64();
        left_pressed_prev = left_pressed;
        right_pressed_prev = right_pressed;
       
        // a queue of events for this frame
        // Currently, it checks for:
        // > whether or not the left/right mouse button is pressed
        // > the position of the mouse cursor (in pixels)
        // > whether or not the x button was pressed on the game window
        while(SDL_PollEvent(&event) != 0){ 
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
        SDL_GetWindowSize(window, &screen_width, &screen_height);

        // The following changes the current display depending on the state of the game
        // this if/else chain controls what shows up on the game window.

        if(*menu_status == HOME){ // Homescreen
            get_home_menu(menu_status, renderer,&mouse_rect,bg,*b1,*b2,*b3,&keep_running,left_pressed);
        }else if(*menu_status == SETTINGS){ // Settings Page
            get_settings_menu(menu_status, renderer, &mouse_rect, settings_bg, *b4, fx, music, left_pressed, *on1, *off1);
        }else if(*menu_status == WAITING){ // Loading screen
            color = 1;
            settings_bg.render();
            loading->render();
            SDL_RenderPresent(renderer);
            color = join_game(renderer);
            if(color == BLACK){
                test_camera.current_y_pos = HEIGHT-15;
                team_image = &black_img;
            }else{
                team_image = &white_img;
            }
            the_game_status = PLACEMENT;
            *menu_status = GAME;
            //get_selection_menu(menu_status, renderer, &test_camera, mouse_x, mouse_y);
            
        }else{ // this block controls the gameplay for each client once two players are in a game.
            // SECTION 1: PIECE SELECTION AND PLACEMENT
            camera_display(test_camera.current_x_pos, test_camera.current_y_pos, &mouse_rect, left_pressed, color, blank);
            test_camera.change_camera_pos(mouse_x, mouse_y);
            // Square s is the square that the mouse is currently over.
            Square* s = get_square(((mouse_x / 32)+test_camera.current_x_pos), ((mouse_y / 32) + test_camera.current_y_pos));
            // If we clicked the square and there's a piece on it,
            // deselect the current piece and select the new piece
            if(s->occupied){
                if(s->piece->button->was_clicked()){
                    if(current_selected_piece != nullptr){
                        current_selected_piece->is_selected = false;
                    }
                    current_selected_piece = s->piece;
                    current_selected_piece->is_selected = true;
                    current_selected_piece->button->current_picture = current_selected_piece->button->clicked_picture;
                    current_selected_button = nullptr;
                }
            }

            // if there is a piece selected this frame, display info about that piece    
            if(current_selected_piece != nullptr){
                sidebar.render(); // display sidebar
                if(current_selected_piece->color == WHITE) sidebar_white.render(); // white flag
                else sidebar_black.render(); // black flag
                hp_img.render(); // heart image
                armor_img.render(); // shield image
                // the rest shows the health and armor values (maxhp is currently hardcoded to 4 or 1)
                unit_type[(int)current_selected_piece->type]->render();
                hpstatus[(int)current_selected_piece->health]->render();
                armorstatus[(int)current_selected_piece->armor]->render();
                slash.render();
                if(current_selected_piece->type == RIFLEMAN) maxhp[4]->render();
                else maxhp[1]->render();
            }

            sidebar2.render(); // the bar on the bottom (this is always present)
            
            // force selected buttons to display their clicked picture.
            if(current_selected_button != nullptr && the_game_status != PLACEMENT){
                current_selected_button->update_button(left_pressed, &mouse_rect);
                current_selected_button->current_picture = current_selected_button->clicked_picture;
                current_selected_button->render(-1, -1, false);
            }

            // a right click will deselect anything selected.
            if(right_pressed){
                current_selected_button = nullptr;
                if(current_selected_piece != nullptr){
                    current_selected_piece->is_selected = false;
                    current_selected_piece = nullptr;
                }
            }

            // more for the placement phase
            if(the_game_status == PLACEMENT){
                select_ctrl.render(); // this is the blue box at the bottom
                //if one of the placement buttons is selected, force it to show its clicked picture.
                //placement buttons are the piece icons that are in the blue box.
                if(current_selected_button != nullptr){
                    current_selected_button->update_button(left_pressed, &mouse_rect);
                    current_selected_button->current_picture = current_selected_button->clicked_picture;
                    current_selected_button->render(-1, -1, false);
                }
                for(Button2* placement_button : placement_buttons){ // check if each placement button is clicked
                    placement_button->update_button(left_pressed, &mouse_rect);
                    placement_button->render();
                    if(placement_button->was_clicked() && placement_button->type != "paratrooper"){
                        current_selected_button = placement_button;
                        std::cout << "changed selected piece" << std::endl;
                        if(current_selected_piece != nullptr){ // deselect currently selected piece if there is one.
                            current_selected_piece->is_selected = false;
                            current_selected_piece = nullptr;
                        }
                    }
                }

                // if we have a selected placement button and click on the map, create a new piece.
                // the square must be "valid" (within the octogon) and the square can't be occupied already.
                if(left_pressed && !right_pressed && current_selected_button != nullptr && s->occupied == false && s->is_valid){
                    // create new Piece
                    int type = KING;
                    if(current_selected_button->type == "warning") type = RIFLEMAN;
                    else if(current_selected_button->type == "king") type = KING;
                    else if(current_selected_button->type == "engineer") type = ENGINEER;
                    else if(current_selected_button->type == "scout") type = SCOUT;
                    else if(current_selected_button->type == "searchlight") type = SEARCHLIGHT;
                    else if(current_selected_button->type == "guard") type = GUARD;
                    else if(current_selected_button->type == "rifleman") type = RIFLEMAN;
                    else if(current_selected_button->type == "specops") type = SPECOPS;
                    else if(current_selected_button->type == "paratrooper") type = PARATROOPER;
                    Piece* p = create_piece(type, color);

                    if(p == NULL){
                        std::cout << "no more pieces of this type." << std::endl;
                    }else{
                        if(type == PARATROOPER) p->button = new Button2(renderer, "paratrooper", 0,0,32,32);
                        else if (type == KING) p->button = new Button2(renderer, "king", 0,0,32,32);
                        else if (type == ENGINEER) p->button = new Button2(renderer, "engineer", 0,0,32,32);
                        else if (type == SCOUT) p->button = new Button2(renderer, "scout", 0,0,32,32);
                        else if (type == SEARCHLIGHT) p->button = new Button2(renderer, "searchlight", 0,0,32,32);
                        else if (type == GUARD) p->button = new Button2(renderer, "guard", 0,0,32,32);
                        else if (type == RIFLEMAN) p->button = new Button2(renderer, "rifleman", 0,0,32,32);
                        else if (type == SPECOPS) p->button = new Button2(renderer, "specops", 0,0,32,32);
                        else p->button = new Button2(renderer, "warning", 0,0,32,32);

                        // check if the piece was placed on the player's side of the board, delete the piece if its not.
                        if(place_piece(p, ((mouse_rect.x / 32)+test_camera.current_x_pos), ((mouse_rect.y / 32) + test_camera.current_y_pos), true)){
                            std::cout << ((mouse_rect.x / 32)+test_camera.current_x_pos)<< std::endl;
                        }else{
                            std::cout << "error: couldn't place piece - invalid placement" << std::endl;
                            delete_piece(p);
                        }
                    }
                    current_selected_button = nullptr;
                }

                // once the "play" button is clicked, hang until another player is ready.
                // then, update the board and move on.
                placement_ready->update_button(left_pressed, &mouse_rect);
                placement_ready->render();
                if(placement_ready->was_clicked()){
                    placement_phase();
                    printf("done placements\n");
                    update_board(renderer);
                    printf("both players ready\n");
                    the_game_status = MOVEMENT_PHASE;
                    current_selected_button = nullptr;
                    printf("changed to movement phase\n");
                }
            }

            // SECTION 2: PIECE MOVEMENT (locally)

            if(the_game_status == MOVEMENT_PHASE){
                your_team->render();
                team_image->render();
                end_turn->update_button(left_pressed, &mouse_rect);
                end_turn->render();
                // if a piece is selected and we click another square, try to move it.
                if(current_selected_piece != nullptr && left_pressed){
                    int sqx = (mouse_rect.x / 32)+test_camera.current_x_pos; // position of x square clicked
                    int sqy = (mouse_rect.y / 32)+test_camera.current_y_pos; // position of y square clicked
                    Square* sq = get_square(sqx, sqy); // the square that was clicked
                    // the one line commented under this one would limit movement to only visible squares.
                    //if(((color == 0 && sq->visibleWhite) || (color == 1 && sq->visibleBlack)) && sq->is_valid){

                    // if the piece belongs to the player, and the square is a valid spot to move, move the piece
                    if(sq->is_valid && sqx > 0 && sqy > 0 && sqx < HEIGHT && sqy < HEIGHT && current_selected_piece->color == color){
                        // this statement will block movement of greater than 6 squares in any direction.
                        if(abs(current_selected_piece->x - sqx) < 6 && abs(current_selected_piece->y - sqy) < 6){
                            place_piece(current_selected_piece, sqx, sqy, false);
                            current_selected_piece->is_selected = false;
                            current_selected_piece = nullptr;
                            printf("moved a piece\n");
                        }
                    }

                }
            }
        } // the end of the giant else block

        SDL_RenderPresent(renderer); // This will actually update what the player can see.

        // a short delay after each frame to lock in the frame rate
        frame_duration = SDL_GetTicks64() - frame_start_time;
        if(frame_delay > frame_duration){
            SDL_Delay(frame_delay - frame_duration);
        }
    }

    if(the_game_status != NOT_ON){
        printf("shutting down\n");
        //end_game();
        std::cout << "Client process shutdown\n";
    }
    

    bg.free_picture();
    select_ctrl.free_picture();
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
    TTF_CloseFont(font);
    SDL_DestroyWindow(window);
    SDL_Quit();

	end_game();
	return 0;
}

int init_png_and_font(){
    if(IMG_Init(IMG_INIT_PNG) == 0){
        return -1;
    }
    if(TTF_Init() == -1){
        return -1;
    }

    return 0;
}

// The first page displayed upon starting the client. 

int get_home_menu(menu* status, SDL_Renderer* renderer, SDL_Rect* mouse_pos, Picture bg, Button2 b1, Button2 b2, Button2 b3, bool* keep_running, bool clicked){
    bg.render();
    b1.update_button(clicked, mouse_pos);
    b1.render();
    b2.update_button(clicked, mouse_pos);
    b2.render();
    b3.update_button(clicked, mouse_pos);
    b3.render();

    if(b1.was_clicked()){ // b1 is the join game button
        *status = WAITING;
    }else if(b2.was_clicked()){ // b2 is the settings button
        *status = SETTINGS;
    }else if(b3.was_clicked()){ // b3 is the quit button
        *keep_running = false;
    }

    return 0;
}

// Show the settings menu. 

int get_settings_menu(menu* status, SDL_Renderer* renderer, SDL_Rect* mouse_pos, Picture bg, Button2 back_button, setting* fx, setting* music, bool clicked, Button2 on1, Button2 off1){
    bg.render();
    back_button.update_button(clicked, mouse_pos);
    back_button.render();
    if(back_button.was_clicked()){
        *status = HOME;
    }
    // The following handles the on/off button, which is actually two separate buttons.
    // in the game window, nothing happens, but it does change the variable fx.
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

// An old function no longer used

int get_selection_menu(menu* status, SDL_Renderer* renderer, Camera* camera, int mouse_x, int mouse_y){
    return 0;
}
