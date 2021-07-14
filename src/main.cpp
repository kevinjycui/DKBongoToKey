#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <windows.h>

#include "../inc/main.h"
#include "../inc/keyboard.h"
#include "../inc/mixer.h"


Mix_Music *gMusic = NULL;
Mix_Chunk *gSFXs[3] = { NULL };

int main(int argc, char ** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(2);
    }

    initMixer();

    if (loadMusic("assets/music.wav", &gMusic) != 0) {
        printf("Failed to load music\n");
        exit(2);
    }

    if (loadSFX("assets/left_hit.wav", &gSFXs[0]) < 0 || 
        loadSFX("assets/right_hit.wav", &gSFXs[1]) < 0 || 
        loadSFX("assets/ok.wav", &gSFXs[2]) < 0 ||
        loadSFX("assets/clap.wav", &gSFXs[3]) < 0 ||
        loadSFX("assets/grunt.wav", &gSFXs[4]) < 0) {
        printf("Failed to load sound effects\n");
        exit(2);
    }

    printf("%i joysticks were found.\n\n", SDL_NumJoysticks());
    printf("The names of the joysticks are:\n");
		
    for(int i=0; i < SDL_NumJoysticks(); i++) {
        printf("\t%d %s\n", i, SDL_JoystickName(SDL_JoystickOpen(i)));
    }

    printf("\n\n=== START INPUT ===\n\n> ");

    SDL_Event event;

    bool runstate = true;
    int prevAxisValue = 5000;

    Mix_PlayMusic(gMusic, -1);

    while (runstate)
    {
        while(SDL_PollEvent(&event))
        {  
            switch(event.type)
            {
                case SDL_JOYBUTTONDOWN:
                    handleButtonEvent(event);
                    if (event.jbutton.button == 9) Mix_PlayChannel(-1, gSFXs[2], 0);
                    else Mix_PlayChannel(-1, gSFXs[event.jbutton.button < 2], 0);
                    break;
                case SDL_JOYAXISMOTION:
                    if (prevAxisValue != 5000 && std::abs(prevAxisValue - event.jaxis.value) > 20000) {
                        printf("C");
                        if (digit == 4) {
                            Mix_PlayChannel(-1, gSFXs[3], 0);
                        }
                        else {
                            Mix_PlayChannel(-1, gSFXs[4], 0);
                        }
                    }
                    prevAxisValue = event.jaxis.value;
                    break;
            }
        }

    }

    SDL_Quit();

    printf("Terminated\n");

    return 0;
}
