#include "../inc/main.h"
#include "../inc/keycodes.h"

#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include <windows.h>

const int KEYEVENT_KEYUP = 0x02;


int main(int argc, char ** argv) 
{
    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0)
    {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    printf("%i joysticks were found.\n\n", SDL_NumJoysticks());
    printf("The names of the joysticks are:\n");
		
    for(int i=0; i < SDL_NumJoysticks(); i++) {
        printf("\t%d %s\n", i, SDL_JoystickName(SDL_JoystickOpen(i)));
    }

    printf("\n\n=== START INPUT ===\n\n> ");

    SDL_Event event;

    bool runstate = true;

    unsigned int digit = 4;
    unsigned int value = 0;

    bool hold = false;
    std::vector<unsigned int> holdqueue;

    while (runstate)
    {
        while(SDL_PollEvent(&event))
        {  
            switch(event.type)
            {
                case SDL_JOYBUTTONDOWN:
                    printf("%d", event.jbutton.button);
                    if (event.jbutton.button == 9) {
                        hold = !hold;
                        if (!hold) {
                            for (unsigned int key : holdqueue)
                                keybd_event(key, 0, KEYEVENT_KEYUP, 0);
                            holdqueue.clear();
                        }
                        break;
                    }
                    digit--;
                    value += event.jbutton.button * pow(4, digit);
                    if (digit == 0) {
                        printf("\n");
                        if (printKeyName(value)) {
                            keybd_event(value, 0, 0, 0);
                            if (!hold)
                                keybd_event(value, 0, KEYEVENT_KEYUP, 0);
                            else
                                holdqueue.push_back(value);
                        }
                        else printf("Invalid keycode\n");
                        digit = 4;
                        value = 0;
                        printf("> ");
                    }
                    break;
                case SDL_JOYAXISMOTION:
                    if (event.jaxis.value > -7000)
                        printf("C");
                    break;
            }
        }

    }

    SDL_Quit();

    printf("Terminated\n");

    return 0;
}
