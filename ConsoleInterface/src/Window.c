#include <stdlib.h>
#include "Window.h"

Window* CreateMainWindow(void(*exitMethod)(), void(*decodeMethod)())
{
	const int buttonCount = 2;
	Window* window = (Window*)malloc(sizeof(Window));
	if (window != NULL) {
		window->ButtonCount = buttonCount;
		Button* buttons = (Button*)malloc(sizeof(Button) * buttonCount);
		window->Buttons = buttons;
		if (buttons != NULL)
		{	
			InitButton(buttons, "DECODE PNG", decodeMethod);
			InitButton(buttons + 1, "EXIT", exitMethod);
		}
		window->Cursor = 0;
	}
	return window;
}

void MoveCursor(Window* window, int direction)
{
	if (window != NULL)
	{
#define value window->Cursor
#define count window->ButtonCount
		value += direction;
		if (value < 0)
			value = count - 1;
		else if (value >= count)
			value %= count;
	}
}

void PressSelectedButton(Window* window)
{
	if (window != NULL)
	{
		int cursor = window->Cursor;
		if(window->Buttons != NULL)
			(window->Buttons + cursor)->Action();
	}
}

void PrintWindow(Window* window)
{
	if (window != NULL && window->Buttons != NULL)
	{
		for (int i = 0; i < window->ButtonCount; i++)
		{
			char cursor = i == window->Cursor ? '*' : ' ';
			printf("[%c]%s\n", cursor, window->Buttons[i].Name);
		}
	}
}