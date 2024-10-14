#pragma once
#include <iostream>
#include <chrono>
#include "resource.h"
using namespace std;


struct TableStructure {
	short SRow;
	short SColumn;
	short SNumofBom;
	short SNumofUnlockedCells;
	short SNumofFlag;
};
	
struct CellStructure {
	bool BHasBom = false;
	bool BUnlocked = false;
	bool BFlagged = false;
	short SAdjacantBoms = 0;
};

struct Timer {
	chrono::high_resolution_clock::time_point start_tick, paused_tick;
	double elapsed_time = 0;
	double total_paused_time = 0;
	double last_display = 0;
	double load_time = 0;
	bool isStarted = false;
	bool isPaused = false;

};

struct HighScore {
	short hour;
	short minute;
	short second;
	short day;
	short month;
	short year;
	double elapsed_time;

};
extern short currentPage, index;
extern bool Playing;
