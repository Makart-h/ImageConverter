#pragma once
#include <stdbool.h>
#include "Button.h"

typedef struct
{
	int Cursor;
	int ButtonCount;
	Button* Buttons;
} Window;

void MoveCursor(Window* window, int direction);
void PressSelectedButton(Window* window);
Window* CreateMainWindow(void(*exitMethod)(), void(*decodeMethod)());
void PrintWindow(Window* window);