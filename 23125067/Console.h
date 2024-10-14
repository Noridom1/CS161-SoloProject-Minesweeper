#pragma once
#include <iostream>
#include <Windows.h>
#include <Shlwapi.h>
#include <conio.h>
#include <string>
using namespace std;

void resizeConsole(SHORT width, SHORT height);
void gotoXY(SHORT x, SHORT y);
void setColor(WORD color);
void setBackgroundColor(WORD color);
void SetCellColor(int x, int y, int t_color, int b_color, string name);
void Cursor(BOOL bVisible = TRUE, DWORD dwSize = 25);
void deleteRow(SHORT SStartPos, SHORT SNumberRow);