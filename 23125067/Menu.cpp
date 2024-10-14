
#include <iostream>
#include <fstream>
#include "Function.h"
#include "Console.h"

void DrawMainMenu(short idx){
	system("cls");
	gotoXY(0, 0);
	setColor(15);
	setBackgroundColor(0);
	for (short i = 0; i < Width; ++i) {
		cout << '-';
	}
	cout << endl;
	char title[] = "-- MINESWEEPER --";
	printInCenter(title, 1, 3, 0);
	cout << endl;
	setColor(15);
	setBackgroundColor(0);
	for (short i = 0; i < Width; ++i) {
		cout << '-';
	}
	char MainMenu[] = "- MAIN MENU -";
	char NewGame[] = "    NEW GAME    ";
	char LoadGame[] = "    LOAD GAME    ";
	char HS[] = "    HIGHSCORE   ";
	char Info[] = "   INFORMATION  ";
	char Exit[] = "      EXIT      ";
	char Enter[] = " > Enter to choose < ";
	printInCenter(MainMenu, 5, 15, 0);
	printInCenter(NewGame, 7, ((idx == 7) ? 0 : 15), ((idx == 7) ? 3 : 0));
	printInCenter(LoadGame, 8, ((idx == 8) ? 0 : 15), ((idx == 8) ? 3 : 0));
	printInCenter(HS, 9, ((idx == 9) ? 0 : 15), ((idx == 9) ? 3 : 0));
	printInCenter(Info, 10, ((idx == 10) ? 0 : 15), ((idx == 10) ? 3 : 0));
	printInCenter(Exit, 11, ((idx == 11) ? 0 : 15), ((idx == 11) ? 3 : 0));
	printInCenter(Enter, 13, 15, 3);

	setBackgroundColor(0);
	setColor(15);
	gotoXY(0, 16);
	for (int i = 0; i < Width; ++i)
		cout << '-';
}
void DrawLevelMenu(short idx) {
	system("cls");
	gotoXY(0, 0);
	setColor(15);
	setBackgroundColor(0);
	for (short i = 0; i < Width; ++i) {
		cout << '-';
	}
	cout << endl;
	char title[] = "-- MINESWEEPER --";
	printInCenter(title, 1, 3, 0);
	cout << endl;
	setColor(15);
	setBackgroundColor(0);
	for (short i = 0; i < Width; ++i) {
		cout << '-';
	}
	char str[] =		"- CHOOSE THE LEVEL -";
	char Easy[] =		"      EASY      ";
	char Medium[] =		"     MEDIUM     ";
	char Hard[] =		"      HARD      ";
	char Customize[] =	"     CUSTOM     ";
	char Back[] =		"      BACK      ";
	char Enter[] = " > Enter to choose < ";
	printInCenter(str, 5, 15, 0);
	printInCenter(Easy, 7, ((idx == 7) ? 0 : 15), ((idx == 7) ? 3 : 0));
	printInCenter(Medium, 8, ((idx == 8) ? 0 : 15), ((idx == 8) ? 3 : 0));
	printInCenter(Hard, 9, ((idx == 9) ? 0 : 15), ((idx == 9) ? 3 : 0));
	printInCenter(Customize, 10, ((idx == 10) ? 0 : 15), ((idx == 10) ? 3 : 0));
	printInCenter(Back, 11, ((idx == 11) ? 0 : 15), ((idx == 11) ? 3 : 0));
	printInCenter(Enter, 13, 15, 3);
	setBackgroundColor(0);
	setColor(15);
	gotoXY(0, 16);
	for (int i = 0; i < Width; ++i)
		cout << '-';
	if (idx == 7) {
		char info[] = "Map size: 9 x 9 | Number of boms: 10";
		printInCenter(info, 17, 15, 0);
	}
	else if (idx == 8) {
		char info[] = "Map size: 15 x 15 | Number of boms: 25";
		printInCenter(info, 17, 15, 0);
	}
	else if (idx == 9) {
		char info[] = "Map size: 19 x 19 | Number of boms: 70";
		printInCenter(info, 17, 15, 0);
	}
	else if (idx == 10) {
		char info[] = "Customize your own map - score will not be recorded";
		printInCenter(info, 17, 15, 0);
	}
	else if (idx == 11) {
		char info[] = "Back to Main Menu";
		printInCenter(info, 17, 15, 0);
	}

}

void DrawEasy() {
	system("cls");
	Initialize(9, 9, 10);
}

void DrawMedium() {
	system("cls");
	Initialize(15, 15, 25);
}

void DrawHard() {
	system("cls");
	Initialize(20, 19, 70);
}

void DrawCustom(short row, short column, short bom) {
	system("cls");
	Initialize(row, column, bom);
}

void DrawModeOfPlaying(int idx) {
	switch (idx) {
	case 7:
		DrawEasy();
		break;
	case 8:
		DrawMedium();
		break;
	case 9:
		DrawHard();
		break;
	case 10:
		short row, column, bom;
		deleteRow(17, 1);
		gotoXY(0, 17);
		setBackgroundColor(0);
		setColor(15);
		cout << "Type the number of Rows (minimum: 1 and maximum: 20): ";
		cin >> row;
		while (row <= 0 || row >= 21) {
			cout << "INVALID! PLease type again: ";
			cin >> row;
		}
		cout << "Type the nunmber of Columns (minimum: 1 and maximum: 40): ";
		cin >> column;
		while (column <= 0 || column >= 41) {
			cout << "INVALID! PLease type again: ";
			cin >> column;
		}
		cout << "Type the number of Boms: ";
		cin >> bom;
		while (bom >= column * row || bom <= 0) {
			if (bom >= column * row)
				cout << "The number of Boms exceed the size of the map. Reduce the number of Boms:";
			else
				cout << "The map must have at least 1 bom: ";
			cin >> bom;
		}
		DrawCustom(row, column, bom);
		break;
	}
	SetStateOfPlaying(1);
}

void DrawEscape(short index) {
	deleteRow(17, 2);
	char str[] = "Are you sure you want to Exit?\n";
	printInCenter(str, 17, 15, 0);
	SetCellColor(Width / 2 - 5, 18, ((index == 12) ? 0 : 15), ((index == 12) ? 3 : 0), "YES");
	SetCellColor(Width / 2 + 2, 18, ((index == 13) ? 0 : 15), ((index == 13) ? 3 : 0), "NO");
	setBackgroundColor(0);
	setColor(15);
}

void DrawHighScoreMenu(short idx) {
	system("cls");
	gotoXY(0, 0);
	setColor(15);
	setBackgroundColor(0);
	for (short i = 0; i < Width; ++i) {
		cout << '-';
	}
	cout << endl;
	char title[] = "-- MINESWEEPER --";
	printInCenter(title, 1, 3, 0);
	cout << endl;
	setColor(15);
	setBackgroundColor(0);
	for (short i = 0; i < Width; ++i) {
		cout << '-';
	}
	char HighScore[] = "HIGHSCORE";
	char ChooseTheMode[] = "CHOOSE THE LEVEL";
	char Easy[] = "      EASY      ";
	char Medium[] = "     MEDIUM     ";
	char Hard[] = "      HARD      ";
	char Back[] = "      BACK      ";
	printInCenter(HighScore, 4, 15, 0);
	printInCenter(ChooseTheMode, 5, 15, 0);
	printInCenter(Easy, 7, ((idx == 7) ? 0 : 15), ((idx == 7) ? 3 : 0));
	printInCenter(Medium, 8, ((idx == 8) ? 0 : 15), ((idx == 8) ? 3 : 0));
	printInCenter(Hard, 9, ((idx == 9) ? 0 : 15), ((idx == 9) ? 3 : 0));
	printInCenter(Back, 10, ((idx == 10) ? 0 : 15), ((idx == 10) ? 3 : 0));
	setBackgroundColor(0);
	setColor(15);
	gotoXY(0, 16);
	for (int i = 0; i < Width; ++i)
		cout << '-';
}

void DrawInfo() {
	system("cls");
	gotoXY(0, 0);
	setColor(15);
	setBackgroundColor(0);
	for (short i = 0; i < Width; ++i) {
		cout << '-';
	}
	cout << endl;
	char title[] = "-- MINESWEEPER --";
	printInCenter(title, 1, 3, 0);
	cout << endl;
	setColor(15);
	setBackgroundColor(0);
	for (short i = 0; i < Width; ++i) {
		cout << '-';
	}
	char info[] = "- INFORMATION -";
	printInCenter(info, 4, 3, 0);
	char str[10000];
	ifstream read;
	read.open("Information.txt");
	read.get(str, 10000, '`');
	gotoXY(0, 6);
	setBackgroundColor(0);
	setColor(15);
	cout << str;
	read.close();

	char about[] = "  - ABOUT ME AND THIS PROJECT -  ";
	printInCenter(about, 22, 3, 0);
	char Enter[] = "> Press ENTER to return <";
	printInCenter(Enter, 31, 3, 0);
	
}

