#include <ctime>
#include <queue>
#include <fstream>
#include <chrono>
#include <iomanip>

#include "main.h"
#include "Function.h"
#include "Console.h"
#include "Menu.h"

TableStructure TableStruct;
CellStructure** CellStruct;
Timer calTime; //Manage the time of playing
short nHSCore; // Number of High Scores saved
HighScore HSCore[10]; //Store the HighScore;
short tmp; //Save the previous state of index

COORD CursorPosition; //Coordinate of the Cursor

bool shouldSave = true;
short TableCOORD_x = 0, TableCOORD_y = 0; //Coordinate of the Table

void getTableCoordinates() {
	TableCOORD_x = (Width) / 2 - TableStruct.SColumn;
	TableCOORD_y = 11;
}

void CreateTable() {
	CellStruct = new CellStructure* [TableStruct.SRow];
	for (short i = 0; i < TableStruct.SRow; i++) {
		CellStruct[i] = new CellStructure[TableStruct.SColumn];
	}
}

void DeleteTable() {
	for (short i = 0; i < TableStruct.SRow; ++i) {
		delete[] CellStruct[i];
	}
	delete[] CellStruct;
}

void Initialize(short Row, short Column, short Bom) {
	TableStruct.SRow = Row;
	TableStruct.SColumn = Column;
	TableStruct.SNumofBom = Bom;
	TableStruct.SNumofUnlockedCells = 0;
	TableStruct.SNumofFlag = 0;
	CreateTable();
	RandomizeBoms();
	CreateCells();
	getTableCoordinates();
	CursorPosition.X = 0;
	CursorPosition.Y = 0;
	DrawTable();
	shouldSave = true;
}

void CreateCells() {
	short dx[8] = {0, 0, -1, -1, -1, 1, 1, 1};
	short dy[8] = {1, -1, 1, -1, 0, 1, -1, 0};

	for (int i = 0; i < TableStruct.SRow; ++i) {
		for (int j = 0; j < TableStruct.SColumn; ++j) {
			if (CellStruct[i][j].BHasBom) {
				CellStruct[i][j].SAdjacantBoms = 9;
				continue;
			}
			for (int k = 0; k < 8; ++k) {
				short DX = i + dx[k];
				short DY = j + dy[k];
				if (DX >= 0 && DX < TableStruct.SRow && DY >= 0 && DY < TableStruct.SColumn && CellStruct[DX][DY].BHasBom) {
					CellStruct[i][j].SAdjacantBoms++;
				}
			}
		}
	}
}

short CoordinateX(short x) {
	return (x * 2) + TableCOORD_x;
}

short CoordinateY(short y) {
	return y + TableCOORD_y;
}

void DrawCell(short x, short y, short type) {
	switch (type)
	{
	case 0: // Empty Cell
		SetCellColor(CoordinateX(x), CoordinateY(y), 0, ((CursorPosition.X == x && CursorPosition.Y == y) ? 0 : 15), "  ");
		break;
	case 1: // 1 Adjacant bom
		SetCellColor(CoordinateX(x), CoordinateY(y), ((CursorPosition.X == x && CursorPosition.Y == y) ? 15 : 1), ((CursorPosition.X == x && CursorPosition.Y == y) ? 0 : 15), "1 ");
		break;
	case 2:	// 2 Adjacant bom
		SetCellColor(CoordinateX(x), CoordinateY(y), ((CursorPosition.X == x && CursorPosition.Y == y) ? 15 : 2), ((CursorPosition.X == x && CursorPosition.Y == y) ? 0 : 15), "2 ");
		break;
	case 3:	// 3 Adjacant bom
		SetCellColor(CoordinateX(x), CoordinateY(y), ((CursorPosition.X == x && CursorPosition.Y == y) ? 15 : 3), ((CursorPosition.X == x && CursorPosition.Y == y) ? 0 : 15), "3 ");
		break;
	case 4:	// 4 Adjacant bom
		SetCellColor(CoordinateX(x), CoordinateY(y), ((CursorPosition.X == x && CursorPosition.Y == y) ? 15 : 4), ((CursorPosition.X == x && CursorPosition.Y == y) ? 0 : 15), "4 ");
		break;
	case 5:	// 5 Adjacant bom
		SetCellColor(CoordinateX(x), CoordinateY(y), ((CursorPosition.X == x && CursorPosition.Y == y) ? 15 : 5), ((CursorPosition.X == x && CursorPosition.Y == y) ? 0 : 15), "5 ");
		break;
	case 6:	// 6 Adjacant bom
		SetCellColor(CoordinateX(x), CoordinateY(y), ((CursorPosition.X == x && CursorPosition.Y == y) ? 15 : 6), ((CursorPosition.X == x && CursorPosition.Y == y) ? 0 : 15), "6 ");
		break;
	case 7:	// 7 Adjacant bom
		SetCellColor(CoordinateX(x), CoordinateY(y), ((CursorPosition.X == x && CursorPosition.Y == y) ? 15 : 7), ((CursorPosition.X == x && CursorPosition.Y == y) ? 0 : 15), "7 ");
		break;
	case 8:	// 8 Adjacant bom
		SetCellColor(CoordinateX(x), CoordinateY(y), ((CursorPosition.X == x && CursorPosition.Y == y) ? 15 : 8), ((CursorPosition.X == x && CursorPosition.Y == y) ? 0 : 15), "8 ");
		break;
	case 9:	// Bom
		SetCellColor(CoordinateX(x), CoordinateY(y), ((CursorPosition.X == x && CursorPosition.Y == y) ? 12 : 0), ((CursorPosition.X == x && CursorPosition.Y == y) ? 0 : 12), "B ");
		break;
	case 10: // Flagged
		SetCellColor(CoordinateX(x), CoordinateY(y), ((CursorPosition.X == x && CursorPosition.Y == y) ? 15 : 12), ((CursorPosition.X == x && CursorPosition.Y == y) ? 0 : 14), "F ");
		break;
	case 11: // Wrong Flagged
		SetCellColor(CoordinateX(x), CoordinateY(y), 11, 6, "Px");
		break;
	case 12: // True Flagged
		SetCellColor(CoordinateX(x), CoordinateY(y), 12, 14, "B ");
		break;
	case 13: // Cursor Control
		if (CellStruct[y][x].BUnlocked) {
			DrawCell(x, y, CellStruct[y][x].SAdjacantBoms);
		}
		else if (CellStruct[y][x].BFlagged) {
			DrawCell(x, y, 10);
		}
		else SetCellColor(CoordinateX(x), CoordinateY(y), 15, 0, "[]");
		break;
	case 14: // Even Cells
		SetCellColor(CoordinateX(x), CoordinateY(y), 15, 3, "[]");
		break;
	case 15: // Odd Cells
		SetCellColor(CoordinateX(x), CoordinateY(y), 15, 3, "[]");
		break;
	}
}
void DrawTable() {
	//SetCellColor(30, 30, 15, 0, to_string(CursorPosition.X) + " " + to_string(CursorPosition.Y));
	for (short i = 0; i < TableStruct.SRow; ++i) {
		for (short j = 0; j < TableStruct.SColumn; ++j) {
			if (CellStruct[i][j].BFlagged) {
				DrawCell(j, i, 10);
			}
				
			else if (CellStruct[i][j].BUnlocked) {
				DrawCell(j, i, CellStruct[i][j].SAdjacantBoms);
			}
				
			else DrawCell(j, i, 14 + (i + j) % 2);
		}
	}
	DrawCell(CursorPosition.X, CursorPosition.Y, 13);
	
}

void RandomizeBoms() {
	short bom = TableStruct.SNumofBom;
	short i, j;
	srand(time(NULL));
	while (bom) {
		i = rand() % TableStruct.SRow;
		j = rand() % TableStruct.SColumn;
		if (!CellStruct[i][j].BHasBom) {
			CellStruct[i][j].BHasBom = true;
			--bom;
		}
	}
}

//Display the positions of  the bombs - used for checking
void ExtractBoms() {
	setColor(15);
	setBackgroundColor(0);
	for (int i = 0; i < TableStruct.SRow; ++i) {
		for (int j = 0; j < TableStruct.SColumn; ++j) {
			if (CellStruct[i][j].BHasBom) cout << "B ";
			else cout << CellStruct[i][j].SAdjacantBoms << " ";
		}
		cout << endl;
	}
}

void Flagging(short x, short y) {
	if (CellStruct[x][y].BUnlocked) return;
	if (CellStruct[x][y].BFlagged) {
		CellStruct[x][y].BFlagged = false;
		TableStruct.SNumofFlag--;
	}
	else {
		if (TableStruct.SNumofBom <= TableStruct.SNumofFlag) return;
		CellStruct[x][y].BFlagged = true;
		TableStruct.SNumofFlag++;
	}
	DrawTable();
	SetStateOfPlaying(1);
}

void Losing() {
	pauseTimer();
	double totaltime = calTime.last_display;
	Playing = false;
	currentPage = 3;
	index = 12;
	for (short i = 0; i < TableStruct.SRow; ++i) {
		for (short j = 0; j < TableStruct.SColumn; ++j) {
			if (CellStruct[i][j].BFlagged) {
				if (CellStruct[i][j].BHasBom)
					DrawCell(j, i, 12);
				else
					DrawCell(j, i, 11);
			}
			else
				DrawCell(j, i, CellStruct[i][j].SAdjacantBoms);
		}
	}
	SetStateOfPlaying(2);
	shouldSave = false;
}

bool Win() {
	return TableStruct.SNumofUnlockedCells == TableStruct.SColumn * TableStruct.SRow - TableStruct.SNumofBom;
}

void Winning() {
	calTime.load_time = 0;
	stopTimer();
	SetStateOfPlaying(3);
	if (shouldSave) {
		loadHighScore(nHSCore, HSCore, tmp);
		addHighScore(HSCore, nHSCore);
		saveHighScore(nHSCore, HSCore, tmp);
	}
	cout << tmp;
	Playing = false;
	currentPage = 4;
	index = 12;
	for (short i = 0; i < TableStruct.SRow; ++i) {
		for (short j = 0; j < TableStruct.SColumn; ++j) {
			if (CellStruct[i][j].BFlagged) {
				if (CellStruct[i][j].BHasBom)
					DrawCell(j, i, 12);
				else
					DrawCell(j, i, 11);
			}
			else
				DrawCell(j, i, CellStruct[i][j].SAdjacantBoms);
		}
	}
	DeleteGameSaved();
}

void Unlocking(short x, short y) {
	if (!CellStruct[x][y].BUnlocked && !CellStruct[x][y].BFlagged) {
		CellStruct[x][y].BUnlocked = true;
		if (CellStruct[x][y].BHasBom) {
			Losing();
		}
		else {
			TableStruct.SNumofUnlockedCells++;
			if (CellStruct[x][y].SAdjacantBoms != 0) {
				CellStruct[x][y].BUnlocked = true;
			}
			else {
				short dx[8] = { 0, 0, -1, 1, 1, 1,-1, -1 };
				short dy[8] = { -1, 1, 0, 0, 1, -1, 1, -1 };
				for (int i = 0; i < 8; ++i) {
					short DX = x + dx[i];
					short DY = y + dy[i];
					if (DX < TableStruct.SRow && DX >= 0 && DY < TableStruct.SColumn && DY >= 0) {
						Unlocking(DX, DY);
					}
				}
			}
				
		}
	}
}

void OpenCell(short x, short y) {
	if (!CellStruct[x][y].BUnlocked && !CellStruct[x][y].BFlagged) {
		Unlocking(x, y);
		if (Win()) {
			Winning();
		}
		if(Playing) DrawTable();
	}
}
//Handle the key event from the user.
void KeyHandling(KEY_EVENT_RECORD key) {
	if (key.bKeyDown) {
		switch (currentPage)
		{
		case 0: //Main Menu
			switch (key.wVirtualKeyCode)
			{
			case VK_UP:
				if (index != 12 && index != 13) {
					index = ((index == 7) ? 11 : index - 1);
					DrawMainMenu(index);
				}
				break;
			case VK_DOWN:
				if (index != 12 && index != 13) {
					index = ((index == 11) ? 7 : index + 1);
					DrawMainMenu(index);
				}
				
				break;
			case VK_RETURN:
				if (index == 7) {
					currentPage = 1;
					DrawLevelMenu(7);
				}
				else if (index == 8) {
					LoadGame();
				}
				else if (index == 9) {
					DrawHighScoreMenu(7);
					currentPage = 5;
					index = 7;
					
				}
				else if (index == 10) {
					DrawInfo();
					currentPage = 7;
				}
				else if (index == 11) {
					DrawEscape(12);
					tmp = index;
					index = 12;
				}
				else if (index == 12) {
					char Bye[] = " GOODBYE! SEE YA! ";
					printInCenter(Bye, 20, 0, 3);
					setBackgroundColor(0);
					setColor(0);
					gotoXY(0, Height - 4);
					exit(1);
				}
				else if (index == 13) {
					deleteRow(22, 2);
					DrawMainMenu(tmp);
					index = tmp;
				}
				break;
			case VK_ESCAPE:
				if (index == 12 || index == 13) {
					index = tmp;
					deleteRow(22, 2);
					DrawMainMenu(index);
				}
				else {
					tmp = index;
					index = 12;
					DrawEscape(index);

				}
				break;
			case VK_RIGHT:
				if (index == 12 || index == 13) {
					index = ((index == 12) ? 13 : 12);
					DrawEscape(index);
				}
				break;
			case VK_LEFT:
				if (index == 12 || index == 13) {
					index = ((index == 12) ? 13 : 12);
					DrawEscape(index);
				}
				break;
			}
			break;
		case 1: //Level Menu
			switch (key.wVirtualKeyCode)
			{
			case VK_UP:
				index = ((index == 7) ? 11 : index - 1);
				DrawLevelMenu(index);
				break;
			case VK_DOWN:
				index = ((index == 11) ? 7 : index + 1);
				DrawLevelMenu(index);
				break;
			case VK_RIGHT:
				if (index == 12 || index == 13) {
					index = ((index == 12) ? 13 : 12);
					DrawEscape(index);
				}
				break;
			case VK_LEFT:
				if (index == 12 || index == 13) {
					index = ((index == 12) ? 13 : 12);
					DrawEscape(index);
				}
				break;
			case VK_RETURN:
				if (index >= 7 && index <= 10) {
					currentPage = 2;
					tmp = index;
					calTime.load_time = 0;
					DrawModeOfPlaying(index);
					initializeTimer();
					
				}
				else if (index == 11) {
					currentPage = 0;
					DrawMainMenu(7);
					index = 7;
				}
				else if (index == 12) {
					char Bye[] = " GOODBYE! SEE YA! ";
					printInCenter(Bye, 20, 0, 3);
					setBackgroundColor(0);
					setColor(0);
					gotoXY(0, Height - 4);
					exit(1);
				}
				else if (index == 13) {
					deleteRow(22, 2);
					DrawLevelMenu(tmp);
					index = tmp;
				}
				break;
			case VK_ESCAPE:	
				if (index == 12 || index == 13) {
					index = tmp;
					deleteRow(22, 2);
					DrawLevelMenu(index);
				}
				else {
					tmp = index;
					index = 12;
					DrawEscape(index);
				}
				break;
			}
			break;
		case 2: //Playing
			switch (key.wVirtualKeyCode)
			{
			case VK_UP:
				if (index != 12 && index != 13) {
					CursorPosition.Y = ((CursorPosition.Y == 0) ? TableStruct.SRow - 1 : CursorPosition.Y - 1);
					DrawTable();
				}
				break;
			case VK_DOWN:
				if (index != 12 && index != 13) {
					CursorPosition.Y = ((CursorPosition.Y == TableStruct.SRow - 1) ? 0 : CursorPosition.Y + 1);
					DrawTable();
				}
				break;
			case VK_LEFT:
				if (index != 12 && index != 13) {
					CursorPosition.X = ((CursorPosition.X == 0) ? TableStruct.SColumn - 1 : CursorPosition.X - 1);
					DrawTable();
				}
				else {
					index = ((index == 12) ? 13 : 12);
					DrawEscapePlaying(index);
				}
				break;
			case VK_RIGHT:
				if (index != 12 && index != 13) {
					CursorPosition.X = ((CursorPosition.X == TableStruct.SColumn - 1) ? 0 : CursorPosition.X + 1);
					DrawTable();
				}
				else {
					index = ((index == 12) ? 13 : 12);
					DrawEscapePlaying(index);
				}
				break;
			case VK_SPACE: // Space: Unlock cell
				if (!calTime.isStarted) {
					startTimer();
				}
				OpenCell(CursorPosition.Y, CursorPosition.X);
				break;
			case 0x46: // F: Flag cell
				if (!calTime.isStarted) {
					calTime.isStarted = true;
					startTimer();
				}
				Flagging(CursorPosition.Y, CursorPosition.X);
				break;
			case VK_RETURN:
				if(index == 12){
					index = 7;
					currentPage = 0;
					DrawMainMenu(7);
					SaveGame();
					//calTime.load_time = getTimeElapsed();
					calTime.isStarted = false;
				}
				else if (index == 13) {
					unpauseTimer();
					index = tmp;
					deleteRow(TableCOORD_y + TableStruct.SRow/2 - 1, 2);
					DrawTable();
					DrawTitle(index);
				}
				break;
			case VK_ESCAPE:
				if (index != 12 && index != 13) {
					pauseTimer();
					index = 12;
					deleteRow(TableCOORD_y, TableStruct.SRow);
					DrawEscapePlaying(12);
				}
				else {
					unpauseTimer();
					index = tmp;
					deleteRow(TableCOORD_y + TableStruct.SRow / 2 - 1, 2);
					DrawTitle(index);
					DrawTable();
				}
				break;
			}
			break;
		case 3: //Lose
			switch (key.wVirtualKeyCode)
			{
			case VK_UP:
				index = ((index == 12) ? 16 : index - 1);
				DrawLosing(index);
				break;
			case VK_DOWN:
				index = ((index == 16) ? 12 : index + 1);
				DrawLosing(index);
				break;
			case VK_RETURN:
				if (index == 12) {
					currentPage = 2;
					index = tmp;
					calTime.load_time = 0;
					if (index == 10) {
						system("cls");
						DrawLevelMenu(10);
					}
					DrawModeOfPlaying(index);
					initializeTimer();
					printTime(calTime.elapsed_time);
				}
				else if (index == 13) {
					system("cls");
					SetStateOfPlaying(1);
					DrawTable();
					currentPage = 2;
					index = tmp;
					unpauseTimer();
					char NoScoring[] = "Because you lost, your score will not be counted!";
					printInCenter(NoScoring, TableCOORD_y + TableStruct.SRow + 2, 0, 12);
					break;
				}
				else if (index == 14) {
					currentPage = 1;
					index = 7;
					DrawLevelMenu(index);
					stopTimer();
					calTime.load_time = 0;
					DeleteGameSaved();
				}
				else if (index == 15) {
					currentPage = 0;
					index = 7;
					DrawMainMenu(index);
					stopTimer();
					calTime.load_time = 0;
					DeleteGameSaved();
				}
				else {
					stopTimer();
					calTime.load_time = 0;
					gotoXY(0, Height - 4);
					DeleteGameSaved();
					exit(1);
				}
				break;
			}
			break;
		case 4: //Win
			switch (key.wVirtualKeyCode)
			{
			case VK_UP:
				index = ((index == 12) ? 14 : index - 1);
				DrawWinning(index);
				break;
			case VK_DOWN:
				index = ((index == 14) ? 12 : index + 1);
				DrawWinning(index);
				break;
			case VK_RETURN:
				if (index == 12) {
					currentPage = 1;
					setBackgroundColor(0);
					index = 7;
					DrawLevelMenu(index);
				}
				else if(index == 13) {
					currentPage = 0;
					index = 7;
					DrawMainMenu(index);
				}
				else if (index == 14) {
					gotoXY(0, Height - 4);
					exit(1);
				}
				break;
			}
			break;
		case 5: //High Score Menu
			switch (key.wVirtualKeyCode)
			{
			case VK_UP:
				index = ((index == 7) ? 10 : index - 1);
				DrawHighScoreMenu(index);
				break;
			case VK_DOWN:
				index = ((index == 10) ? 7 : index + 1);
				DrawHighScoreMenu(index);
				break;
			case VK_RIGHT:
				if (index == 12 || index == 13) {
					index = ((index == 12) ? 13 : 12);
					DrawEscape(index);
				}
				break;
			case VK_LEFT:
				if (index == 12 || index == 13) {
					index = ((index == 12) ? 13 : 12);
					DrawEscape(index);
				}
				break;
			case VK_RETURN:
				if (index >= 7 && index <= 9) {
					currentPage = 6;
					displayHighScore(index);
				}
				else if (index == 10) {
					currentPage = 0;
					DrawMainMenu(7);
					index = 7;
				}
				else if (index == 12) {
					setColor(0);
					setBackgroundColor(0);
					exit(1);
				}
				else if (index == 13) {
					deleteRow(22, 2);
					index = tmp;
					DrawHighScoreMenu(index);
				}
				break;
			case VK_ESCAPE:
				if (index == 12 || index == 13) {
					index = tmp;
					deleteRow(22, 2);
					DrawHighScoreMenu(index);
				}
				else {
					tmp = index;
					index = 12;
					DrawEscape(index);
				}
				break;
			}
			break;
		case 6: //HighScore board
			switch (key.wVirtualKeyCode)
			{
			case VK_RETURN:
				index = 7;
				DrawHighScoreMenu(index);
				currentPage = 5;

			}
			break;
		case 7: //Information
			switch (key.wVirtualKeyCode)
			{
			case VK_RETURN:
				index = 7;
				DrawMainMenu(index);
				currentPage = 0;

			}
			break;
		}

	}
}

void EventHandling() {
	while (1) {
		if (calTime.isStarted) {
			calTime.elapsed_time = getTimeElapsed();
			if (calTime.elapsed_time - calTime.last_display >= 1.0) {
				printTime(calTime.elapsed_time);
				calTime.last_display = calTime.elapsed_time;
			}
		}
		DWORD NumOfEvents = 0;
		DWORD NumOfEventsRead = 0;
		HANDLE GetConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
		GetNumberOfConsoleInputEvents(GetConsoleInput, &NumOfEvents);
		if (NumOfEvents) {
			INPUT_RECORD* EventBuffer = new INPUT_RECORD[NumOfEvents];
			ReadConsoleInput(GetConsoleInput, EventBuffer, NumOfEvents, &NumOfEventsRead);

			for (DWORD i = 0; i < NumOfEvents; ++i) {
				if (EventBuffer[i].EventType == KEY_EVENT) {
					KeyHandling(EventBuffer[i].Event.KeyEvent);
				}
			}
		}
	}
	setColor(0);
	setBackgroundColor(0);
}

void SetStateOfPlaying(const short state) {
	switch (state) {
	case 1: //Playing
	{
		deleteRow(4, 1);
		DrawTitle(index);
		printTime(getTimeElapsed());
		Playing = true;
		char instruction[] =	".---------HOW TO PLAY----------.";
		char flag[] =			"|   F: Flag and unflag a cell  |";
		char unlock[] =			"|    SPACEBAR: Unlock a cell   |";
		char move[] =			"|      ARROW KEYS: Moving      |";
		char esc[] =			"|    ESC: Back to main menu    |";
		char goodluck[] =		".__________GOOD LUCK!__________.";
		printInCenter(instruction, TableCOORD_y  - 7, 15, 0);
		printInCenter(flag, TableCOORD_y - 6, 15, 0);
		printInCenter(unlock, TableCOORD_y - 5, 15, 0);
		printInCenter(move, TableCOORD_y - 4, 15, 0);
		printInCenter(esc, TableCOORD_y - 3, 15, 0);
		printInCenter(goodluck, TableCOORD_y - 2, 15, 0);
		break;
	}
	case 2: //Lose
	{
		deleteRow(TableCOORD_y - 7, 6);
		DrawTitle(tmp);
		DrawLosing(12);
		break;
	}
	case 3: //Win
	{
		deleteRow(TableCOORD_y - 7, 6);
		DrawTitle(tmp);
		DrawWinning(12);
		break;
	}
	}
}

void DrawTitle(short idx) {
	gotoXY(0, 0);
	setBackgroundColor(0);
	setColor(15);
	for (short i = 0; i < Width; ++i) {
		cout << '-';
	}
	cout << endl;
	char str[] = "-- MINESWEEPER --";
	printInCenter(str, 1, 3, 0);
	cout << endl;
	setColor(15);
	setBackgroundColor(0);
	for (short i = 0; i < Width; ++i) {
		cout << '-';
	}
	SetCellColor(1, 3, 15, 0, "MAP SIZE: ");
	setBackgroundColor(0);
	cout << TableStruct.SRow << " x " << TableStruct.SColumn;
	SetCellColor(1, 4, 15, 0, "BOMBS REMAINING: ");
	setBackgroundColor(0);
	cout << TableStruct.SNumofBom - TableStruct.SNumofFlag << endl;
	SetCellColor(Width - 20, 3, 15, 0, "TIMER: ");
	switch (idx) {
	case 7:
		SetCellColor(Width - 20, 4, 15, 0, "MODE: Easy");
		break;
	case 8:
		SetCellColor(Width - 20, 4, 15, 0, "MODE: Medium");
		break;
	case 9:
		SetCellColor(Width - 20, 4, 15, 0, "MODE: Hard");
		break;
	case 10:
		SetCellColor(Width - 20, 4, 15, 0, "MODE: Custom");
	}
	
	setBackgroundColor(0);
}

//Print the text in the middle of the screen.
void printInCenter(char str[], short coord_y, short t_color, short b_color) {
	SetCellColor(Width / 2 - strlen(str) / 2, coord_y, t_color, b_color, str);
	setBackgroundColor(0);
	setColor(0);
}

void DrawWinning(short idx) {
	deleteRow(TableCOORD_y + TableStruct.SRow + 1, 2);
	char str1[] = " CONGRATULATIONS ";
	printInCenter(str1, TableCOORD_y + TableStruct.SRow + 1, 14, 3);
	char str2[] = " ! YOU WIN ! ";
	printInCenter(str2, TableCOORD_y + TableStruct.SRow + 2, 14, 3);
	char str3[] = "     NEW GAME     ";
	char str4[] = "   BACK TO MENU   ";
	char str5[] = "       EXIT       ";
	printInCenter(str3, TableCOORD_y - 4, ((idx == 12) ? 0 : 15), ((idx == 12) ? 3 : 0));
	printInCenter(str4, TableCOORD_y - 3, ((idx == 13) ? 0 : 15), ((idx == 13) ? 3 : 0));
	printInCenter(str5, TableCOORD_y - 2, ((idx == 14) ? 0 : 15), ((idx == 14) ? 3 : 0));
}

void DrawLosing(short idx) {
	char str1[] = "YOU LOSE";
	printInCenter(str1, TableCOORD_y + TableStruct.SRow + 1, 0, 12);
	char str2[] = "    PLAY AGAIN    ";
	char str3[] = "     CONTINUE     ";
	char str4[] = "   CHANGE LEVEL   ";
	char str5[] = "   BACK TO MENU   ";
	char str6[] = "       EXIT       ";
	printInCenter(str2, TableCOORD_y - 6, ((idx == 12) ? 0 : 15), ((idx == 12) ? 3 : 0));
	printInCenter(str3, TableCOORD_y - 5, ((idx == 13) ? 0 : 15), ((idx == 13) ? 3 : 0));
	printInCenter(str4, TableCOORD_y - 4, ((idx == 14) ? 0 : 15), ((idx == 14) ? 3 : 0));
	printInCenter(str5, TableCOORD_y - 3, ((idx == 15) ? 0 : 15), ((idx == 15) ? 3 : 0));
	printInCenter(str6, TableCOORD_y - 2, ((idx == 16) ? 0 : 15), ((idx == 16) ? 3 : 0));

}

void DrawEscapePlaying(short index) {
	char str[] = "Are you sure you want to Exit?\n";
	printInCenter(str, TableCOORD_y + TableStruct.SRow / 2 - 1, 15, 0);
	SetCellColor(Width / 2 - 5, TableCOORD_y + TableStruct.SRow / 2, 15, ((index == 12) ? 3 : 0), "YES");
	SetCellColor(Width / 2 + 2, TableCOORD_y + TableStruct.SRow / 2, 15, ((index == 13) ? 3 : 0), "NO");
	setBackgroundColor(0);
	setColor(15);
}

void SaveGame() {
	ofstream write;
	write.open("LoadGame.txt");
	write << getTimeElapsed() << endl;
	write << tmp << endl;
	write << TableStruct.SRow << endl;
	write << TableStruct.SColumn << endl;
	write << TableStruct.SNumofBom << endl;
	write << TableStruct.SNumofFlag << endl;
	write << TableStruct.SNumofUnlockedCells << endl;
	for (int i = 0; i < TableStruct.SRow; ++i) {
		for (int j = 0; j < TableStruct.SColumn; ++j) {
			write << CellStruct[i][j].BFlagged << " " << CellStruct[i][j].BHasBom << " " << CellStruct[i][j].BUnlocked << " " << CellStruct[i][j].SAdjacantBoms << endl;
		}
	}
	write.close();
	DeleteTable();
}

void LoadGame() {
	ifstream read;
	read.open("LoadGame.txt");
	if (!read.is_open()) {
		char str[] = "There is no saved Game";
		printInCenter(str, 17, 15, 0);
		return;
	}
	read >> calTime.load_time;
	if (read.eof()) {
		char str[] = "There is no saved Game";
		printInCenter(str, 17, 15, 0);
		return;
	}
	read >> index;
	read >> TableStruct.SRow;
	read >> TableStruct.SColumn;
	read >> TableStruct.SNumofBom;
	read >> TableStruct.SNumofFlag;
	read >> TableStruct.SNumofUnlockedCells;
	CreateTable();
	for (int i = 0; i < TableStruct.SRow; ++i) {
		for (int j = 0; j < TableStruct.SColumn; ++j) {
			read >> CellStruct[i][j].BFlagged >> CellStruct[i][j].BHasBom >> CellStruct[i][j].BUnlocked >> CellStruct[i][j].SAdjacantBoms;
		}
	}
	read.close();
	system("cls");
	currentPage = 2;
	Playing = true;
	getTableCoordinates();
	SetStateOfPlaying(1);
	initializeTimer();
	printTime(calTime.load_time);
	DrawTable();
}

void DeleteGameSaved() {
	ofstream write;
	write.open("LoadGame.txt", ios::trunc);
	write.close();
}

void printTime(double elapsed_time) {
	int time_to_print = (int)(elapsed_time);
	if (time_to_print / 60 < 10 && time_to_print % 60 < 10)
		SetCellColor(Width - 13, 3, 15, 0, "0" + to_string(time_to_print / 60) + ":0" + to_string(time_to_print % 60));
	else if (time_to_print / 60 < 10) {
		SetCellColor(Width - 13, 3, 15, 0, "0" + to_string(time_to_print / 60) + ":" + to_string(time_to_print % 60));
	}
	else if (time_to_print % 60 < 10) {
		SetCellColor(Width - 13, 3, 15, 0, to_string(time_to_print / 60) + ":0" + to_string(time_to_print % 60));
	}
	else {
		SetCellColor(Width - 13, 3, 15, 0, to_string(time_to_print / 60) + ":" + to_string(time_to_print % 60));
	}
}

void initializeTimer() {
	calTime.total_paused_time = 0;
	calTime.last_display = 0;
	calTime.elapsed_time = 0;
	calTime.isStarted = false;
	calTime.isPaused = false;
}

void startTimer() {
	calTime.isStarted = true;
	calTime.isPaused = false;
	calTime.start_tick = chrono::high_resolution_clock::now();
}

void pauseTimer() {
	if (calTime.isStarted && !calTime.isPaused) {
		calTime.paused_tick = chrono::high_resolution_clock::now();
		calTime.isPaused = true;
	}
}

void unpauseTimer() {
	if (calTime.isPaused) {
		chrono::high_resolution_clock::time_point current = chrono::high_resolution_clock::now();
		chrono::duration<double> paused_duration = current - calTime.paused_tick;
		calTime.total_paused_time += paused_duration.count();
		calTime.isPaused = false;
	}
}

void stopTimer() {
	calTime.isPaused = false;
	calTime.isStarted = false;
}

double getTimeElapsed() {
	if (calTime.isStarted) {
		chrono::high_resolution_clock::time_point current_time = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed_time = current_time - calTime.start_tick;
		if (calTime.isPaused) {
			chrono::duration<double> elapsed_paused_time = calTime.paused_tick - calTime.start_tick;
			return elapsed_paused_time.count() - calTime.total_paused_time + calTime.load_time;
		}
		return elapsed_time.count() - calTime.total_paused_time + calTime.load_time;

	}
	return 0;
}

void writeHSCore(HighScore HSCore, ofstream& file) {
	file << HSCore.hour << " " << HSCore.minute << " " << HSCore.second << " " << HSCore.day << " " << HSCore.month << " " << HSCore.year << " " << HSCore.elapsed_time << endl;
}

void readHSCore(HighScore& HSCore, ifstream& file) {
	file >> HSCore.hour >> HSCore.minute >> HSCore.second >> HSCore.day >> HSCore.month >> HSCore.year >> HSCore.elapsed_time;
}

void addHighScore(HighScore HSCore[], short& nHSCore) {
	chrono::system_clock::time_point now = chrono::system_clock::now();
	time_t current = chrono::system_clock::to_time_t(now);
	tm SavingTime;
	localtime_s(&SavingTime, &current);
	HighScore x;
	x.hour = SavingTime.tm_hour;
	x.minute = SavingTime.tm_min;
	x.second = SavingTime.tm_sec;
	x.day = SavingTime.tm_mday;
	x.month = SavingTime.tm_mon + 1;
	x.year = SavingTime.tm_year + 1900;
	x.elapsed_time = calTime.elapsed_time;
	int idx = nHSCore - 1;
	while (idx >= 0 && HSCore[idx].elapsed_time > x.elapsed_time) {
		HSCore[idx + 1] = HSCore[idx];
		idx--;
	}
	HSCore[idx + 1] = x;
	if(nHSCore < 9) nHSCore++;
}
void saveHighScore(short nHSCore, HighScore HSCore[], short idx) {
	ofstream write;
	if (idx == 7)
		write.open("HighScoreEasy.txt");
	else if (idx == 8)
		write.open("HighScoreMedium.txt");
	else if (idx == 9) write.open("HighScoreHard.txt");
	write << nHSCore << endl;
	for (int i = 0; i < nHSCore; ++i) {
		writeHSCore(HSCore[i], write);
	}
	write.close();
}

void loadHighScore(short& nHSCore, HighScore HSCore[], short idx) {
	ifstream read;
	if (idx == 7)
		read.open("HighScoreEasy.txt");
	else if (idx == 8)
		read.open("HighScoreMedium.txt");
	else if (idx == 9) read.open("HighScoreHard.txt");
	if (!read.is_open()) {
		nHSCore = 0;
		return;
	}
	read >> nHSCore;
	for (int i = 0; i < nHSCore; ++i) {
		readHSCore(HSCore[i], read);
	}
	read.close();
}

void displayHighScore(short idx) {
	system("cls");
	gotoXY(0, 0);
	setColor(15);
	setBackgroundColor(0);
	for (short i = 0; i < Width; ++i) {
		cout << '-';
	}
	cout << endl;
	char title[] = "-- MINESWEEPER --\n";
	printInCenter(title, 1, 3, 0);
	setColor(15);
	setBackgroundColor(0);
	for (short i = 0; i < Width; ++i) {
		cout << '-';
	}
	loadHighScore(nHSCore, HSCore, idx);
	if (idx == 7) {
		char str[] = ".---------------HIGHSCORE OF EASY MODE---------------.";
		printInCenter(str, 5, 15, 0);
	}
	else if (idx == 8) {
		char str[] = ".--------------HIGHSCORE OF MEDIUM MODE--------------.";
		printInCenter(str, 5, 15, 0);
	}
	else if (idx == 9) {
		char str[] = ".---------------HIGHSCORE OF HARD MODE---------------.";
		printInCenter(str, 5, 15, 0);
	}
	gotoXY(13, 6);
	setColor(15);
	setBackgroundColor(0);
	cout << "|";
	gotoXY(66, 6);
	setColor(15);
	setBackgroundColor(0);
	cout << "|";
	setBackgroundColor(0);
	if (nHSCore == 0) {
		char noHS[] = "There is no HighScore saved in this mode";
		printInCenter(noHS, 7 + nHSCore - 1, 15, 0);
	}
	setColor(15);
	for (short i = 0; i < nHSCore; ++i) {
		gotoXY(13, 7 + i);
		cout << "| " << i + 1 << ") ";
		cout.setf(ios::showpoint);
		cout.precision(4);
		cout << HSCore[i].elapsed_time << " seconds | saved at ";
		if (HSCore[i].hour < 10) cout << "0";
		cout << HSCore[i].hour << ":";
		if (HSCore[i].minute < 10) cout << "0";
		cout << HSCore[i].minute << ":";
		if (HSCore[i].second < 10) cout << "0";
		cout << HSCore[i].second << " on ";
		if (HSCore[i].day < 10) cout << "0";
		cout << HSCore[i].day << "/";
		if (HSCore[i].month < 10) cout << "0";
		cout << HSCore[i].month << "/" << HSCore[i].year << " |" << endl;
	}
	gotoXY(13, 7 + nHSCore);
	for (int i = 0; i < 54; ++i){
		if (i == 0 || i == 53)
			cout << ".";
		else cout << "_";
	}
	char Enter[] = "> Press ENTER to return <";
	printInCenter(Enter, 7 + nHSCore + 2, 3, 0);
}
