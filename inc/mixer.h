#include <SDL2/SDL_mixer.h>

void initMixer();

int loadMusic(const char * filename, Mix_Music **gMusic);
int loadSFX(const char * filename, Mix_Chunk **gSFX);