#include <SDL2/SDL.h>

extern unsigned int digit;

int printKeyName(int key_code);
void handleButtonEvent(SDL_Event event);

const int KEYEVENT_KEYUP = 0x02;