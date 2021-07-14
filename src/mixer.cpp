#include <stdio.h>
#include <cstdlib>

#include "../inc/mixer.h"

void initMixer()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "MIX_OpenAudio: %s\n", Mix_GetError());
        exit(2);
    }
}

int loadMusic(const char * filename, Mix_Music **gMusic)
{
	*gMusic = Mix_LoadMUS(filename);
	if (*gMusic == NULL) return 2;
	return 0;
}

int loadSFX(const char * filename, Mix_Chunk **gSFX)
{
	*gSFX = Mix_LoadWAV(filename);
	if (*gSFX == NULL) return 2;
	return 0;
}