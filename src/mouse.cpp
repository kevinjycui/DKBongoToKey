#include <stdio.h>
#include <algorithm>
#include <vector>
#include <windows.h>

#include "../inc/mouse.h"


bool drumState[4] = { false };
bool drumClicked[4] = { false };
int drumMouseMapX[4] = {-16, 16, 0, 0};
int drumMouseMapY[4] = {0, 0, 16, -16};

bool hold_m = false;
std::vector<DWORD> holdqueue_m;

int move_mouse_relative(int x, int y)
{
    POINT current;
    if (!GetCursorPos(&current)) {
        fprintf(stderr, "Failed to get cursor position\n");
        return 1;
    }
    if (!SetCursorPos(current.x + x, current.y + y)) {
        fprintf(stderr, "Failed to set cursor position\n");
        return 1;
    }
    return 0;
}

int click_mouse(DWORD flag)
{
    INPUT Input[1] = {0};
    Input[0].type = INPUT_MOUSE;
    Input[0].mi.dwFlags = flag;
    if (!SendInput(1, Input, sizeof(INPUT))) {
        fprintf(stderr, "Failed to click mouse\n");
        return 1;
    }
    return 0;
}

void close_mouse()
{
    for (DWORD key : holdqueue_m)
        click_mouse(key);
    holdqueue_m.clear();
}

void mouse_handleButtonEvent(SDL_Event event)
{
    printf("%d", event.jbutton.button);
    if (event.jbutton.button == 9) {
        hold_m = !hold_m;
        if (!hold_m) {
            for (DWORD key : holdqueue_m)
                click_mouse(key);
            holdqueue_m.clear();
        }
        return;
    }
    drumState[event.jbutton.button] = true;
    drumClicked[event.jbutton.button] = true;
}

void mouse_handleButtonUpEvent(SDL_Event event)
{
    if (event.jbutton.button != 9) drumState[event.jbutton.button] = false;
}

void runMouseCommands()
{
    DWORD value = 0;
    DWORD reverse = 0;
    bool action = false;
    if (drumState[0] && drumState[1] && drumState[2] && drumState[3]) {
        value = MOUSEEVENTF_MIDDLEDOWN;
        reverse = MOUSEEVENTF_MIDDLEUP;
        std::fill(drumState, drumState + 4, false);
    }
    else {
        if (drumState[0] && drumState[1]) {
            value = MOUSEEVENTF_RIGHTDOWN;
            reverse = MOUSEEVENTF_RIGHTUP;
            drumState[0] = drumState[1] = false;
        }
        if (drumState[2] && drumState[3]) {
            value = MOUSEEVENTF_LEFTDOWN;
            reverse = MOUSEEVENTF_LEFTUP;
            drumState[2] = drumState[3] = false;
        }
    }
    if (value) {
        click_mouse(value);
        if (!hold_m)
            click_mouse(reverse);
        else
            holdqueue_m.push_back(reverse);
        action = true;
    }
    for (int i=0; i<4; i++)
        if (drumState[i] && drumClicked[i]) {
            move_mouse_relative(drumMouseMapX[i], drumMouseMapY[i]);
            drumClicked[i] = false;
            action = true;
        }
    if (!action) return;
    printf("\n");
    if (hold_m) printf("(hold)");
    printf("> ");
}