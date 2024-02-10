// Class to manage buttons on the main menu
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

enum Status { NORMAL, CLICKED, HOVERED };

class Button{
    private:
        string name;
        int water;
        bool active;
        Status status; // current button status
        Status prev_status; //
        SDL_Rect r;
        SDL_Rect* rect;
        //Status stat; 
        
    public:
        int water_function(int business);
        void update_button(bool pressed);
        //enum Status { NORMAL, CLICKED, HOVERED };

        Button(string bname); // constructor

};

