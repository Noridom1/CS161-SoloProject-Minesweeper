#pragma once
#include <chrono>
#include "main.h"

#define Width 80
#define Height 35

//Functions for preparing the game:
void getTableCoordinates();
void CreateTable();
void DeleteTable();
void Initialize(short Row, short Column, short Bom);
void DrawCell(short x, short y, short type);
void DrawTable();
void CreateCells();
void RandomizeBoms();
void ExtractBoms();

//Functions for Playing
void Flagging(short x, short y);
void AutoUnlock(short x, short y);
void Unlocking(short x, short y);
void EventHandling();

//Functions for Changing between states of playing and pages:
void DrawTitle(short idx);
void SetStateOfPlaying(const short state);
void printInCenter(char str[], short coord_y, short t_color, short b_color);
void DrawWinning(short idx);
void DrawLosing(short idx);

//Functions for game saving:
void SaveGame();
void LoadGame();
void DeleteGameSaved();

//Functions for calculating the time of playing:
void printTime(double elapsed_time);
void initializeTimer();
void startTimer();
void pauseTimer();
void unpauseTimer();
void stopTimer();
double getTimeElapsed();

//Functions relating to Highscore:
void writeHSCore(HighScore HSCore, ofstream& file);
void readHSCore(HighScore& HSCore, ifstream& file);
void addHighScore(HighScore HSCore[], short& nHSCore);
void saveHighScore(short nHSCore, HighScore HSCore[], short idx);
void loadHighScore(short& nHSCore, HighScore HSCore[], short idx);
void displayHighScore(short idx);