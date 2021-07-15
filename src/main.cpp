#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <windows.h>
#include <ctime>

#include "../inc/main.h"
#include "../inc/keyboard.h"
#include "../inc/mouse.h"
#include "../inc/mixer.h"


Mix_Music *gMusic = NULL;
Mix_Chunk *gSFXs[5] = { NULL };

std::time_t exitPressed;

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
		
    for(int i=0; i < SDL_NumJoysticks(); i++)
        printf("\t%d %s\n", i, SDL_JoystickName(SDL_JoystickOpen(i)));

    printf("\n\n=== START INPUT ===\n\n> ");

    SDL_Event event;

    bool runstate = true;
    int prevAxisValue = 5000;

    bool kbMouseState = true;

    if (Mix_PlayMusic(gMusic, -1) < 0) fprintf(stderr, "Error playing music: %s\n", Mix_GetError());

    while (runstate)
    {
        while(SDL_PollEvent(&event))
        {  
            switch(event.type)
            {
                case SDL_JOYBUTTONDOWN:
                    if (kbMouseState)
                        kb_handleButtonEvent(event);
                    else
                        mouse_handleButtonEvent(event);

                    if (event.jbutton.button == 9) {
                        exitPressed = std::time(0);
                        Mix_PlayChannel(-1, gSFXs[2], 0);
                    }
                    else if (Mix_PlayChannel(-1, gSFXs[event.jbutton.button < 2], 0) < 0) fprintf(stderr, "Error playing channel: %s\n", Mix_GetError());
                    break;
                case SDL_JOYBUTTONUP:
                    if (!kbMouseState) mouse_handleButtonUpEvent(event);
                    if (event.jbutton.button == 9 && std::time(0) - exitPressed >= 5) runstate = false;
                    break;
                case SDL_JOYAXISMOTION:
                    if (prevAxisValue != 5000 && event.jaxis.value - prevAxisValue > 20000) {
                        printf("C");
                        if (!kbMouseState || digit == 4) {
                            kbMouseState = !kbMouseState;
                            if (Mix_PlayChannel(-1, gSFXs[3], 0) < 0) fprintf(stderr, "Error playing channel: %s\n", Mix_GetError());
                        }
                        else if (Mix_PlayChannel(-1, gSFXs[4], 0) < 0) fprintf(stderr, "Error playing channel: %s\n", Mix_GetError());
                    }
                    prevAxisValue = event.jaxis.value;
                    break;
            }
        }

        if (!kbMouseState) runMouseCommands();

    }

    close_kb();
    close_mouse();

    SDL_Quit();

    printf("\nTerminated\n");

    return 0;
}
