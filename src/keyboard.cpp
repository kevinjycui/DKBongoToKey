#include <stdio.h>
#include <math.h>
#include <vector>
#include <windows.h>

#include "../inc/keyboard.h"


unsigned int digit = 4;
unsigned int value = 0;

bool hold = false;
std::vector<unsigned int> holdqueue;

int printKeyName(int key_code)
{
    char key_name[1024];
    UINT scanCode = MapVirtualKeyW(key_code, MAPVK_VK_TO_VSC);
    LONG lParamValue = (scanCode << 16);

    int rc = GetKeyNameText(lParamValue, key_name, 1024);
    if (rc) printf("%s\n", key_name);
    return rc;
}

void handleButtonEvent(SDL_Event event)
{
    printf("%d", event.jbutton.button);
    if (event.jbutton.button == 9) {
        hold = !hold;
        if (!hold) {
            for (unsigned int key : holdqueue)
                keybd_event(key, 0, KEYEVENT_KEYUP, 0);
            holdqueue.clear();
        }
        return;
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
        if (hold) printf("(hold)");
        printf("> ");
    }
}