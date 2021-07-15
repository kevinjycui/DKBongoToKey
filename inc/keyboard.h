#include <SDL2/SDL.h>

extern unsigned int digit;

int printKeyName(int key_code);
void close_kb();
void kb_handleButtonEvent(SDL_Event event);