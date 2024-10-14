#include "main.h"
#include "Console.h"
#include <string>
#include <iostream>
#include "Function.h"
#include "Menu.h"

using namespace std;

short currentPage , index;
bool Playing;

int main() {
	SetConsoleTitle(L"MINESWEEPER - NGUYEN LE THINH PHUC - 23125067 - 23TT1");
	resizeConsole(Width, Height);
	LoadIcon(NULL, MAKEINTRESOURCE(IDI_MyIcon));
	DrawMainMenu(7);
	index = 7;
	currentPage = 0;
	Playing = true;
	Cursor(false);
	EventHandling();
	
	return 0;
}