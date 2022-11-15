#include <stdlib.h>
#include <string.h>
#include "Button.h"

void InitButton(Button* button, const char* name, void (*method)())
{
	if (button != NULL) {
		size_t length = strlen(name) + 1;
		if (length > 0)
		{
			button->Name = (char*)malloc(length);
			if(button->Name != NULL)
				strcpy_s(button->Name, length, name);
		}
		button->Action = method;
	}
	return button;
}