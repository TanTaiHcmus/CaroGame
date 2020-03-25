#pragma once
#include "Libs.h"

void SetColor(int ForgC)
{
	WORD wColor;
	//This handle is needed to get the current background attribute

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	//csbi is used for wAttributes word

	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		//To mask out all but the background attribute, and to add the color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}

void setcursor(bool visible) // set bool visible = 0 - invisible, bool visible = 1 - visible
{
	CONSOLE_CURSOR_INFO lpCursor;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void Gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { y, x };
	SetConsoleCursorPosition(h, c);
}

bool FindNullPos(int PlayerCom[MaxN][MaxN]) {
	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Column; j++)
			if (PlayerCom[i][j] == 0) {
				return true;
			}
	return false;
}