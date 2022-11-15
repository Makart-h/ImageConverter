#pragma once
#include <stdio.h>

typedef struct
{
	const char* Name;
	void (*Action)();
} Button;

void InitButton(Button* button, const char* name, void (*method)());