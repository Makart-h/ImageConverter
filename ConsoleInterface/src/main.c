#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include "Window.h"
#include "KeyboardInput.h"

bool IsRunning = true;
FILE* image;

void Exit();
void HandleInput(Window* window, int keyCode);
void HandlePNG();
errno_t ReadFilePath();

int main(int argc, char** argv)
{	
	errno_t error = ReadFilePath();
	if (error == 0)
	{
		system("cls");
		Window* window = CreateMainWindow(&Exit, &HandlePNG);
		PrintWindow(window);
		while (IsRunning)
		{			
			if (_kbhit())
			{
				int keyCode = _getch();
				HandleInput(window, keyCode);
				if (IsRunning)
				{
					system("cls");
					PrintWindow(window);
				}
			}
		}
	}
	else
	{
		printf("Error opening the file!");
	}
}

errno_t ReadFilePath()
{
	char path[FILENAME_MAX];
	printf("Filepath: ");
	gets_s(path, FILENAME_MAX);
	return fopen_s(&image, path, "rb");
}

void HandleInput(Window* window, int keyCode)
{
	switch (keyCode)
	{
	case ARROW:
		keyCode = _getch();
		if (keyCode == UP_ARROW)
			MoveCursor(window, UP);
		else if (keyCode == DOWN_ARROW)
			MoveCursor(window, DOWN);
		break;
	case ENTER:
		system("cls");
		PressSelectedButton(window);
		break;
	case ESC:
		IsRunning = false;
		break;
	}
}

void HandlePNG()
{
	if (DecodePNG(image))
		printf("File decoded succesfully!");
	else
		printf("Error during decoding!");

	Exit();
}

void Exit()
{
	fclose(image);
	IsRunning = false;
}