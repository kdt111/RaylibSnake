#pragma once
#include "raylib.h"

//Game configuration
namespace Config
{
	const int GAME_SIZE = 800; //Both vertical and horizontal size of the screen
	const int CELL_SIZE = 20; //How many pixels does each cell of the game take
	const int TARGET_FRAMERATE = 15; //How fast should the snake move
	const Color CLEAR_COLOR = WHITE; //To what color should the screen be cleard
	bool PLAYING = false; //Check if the menu or the game should be updated
	const int FONT_SIZE = 30; //Font size of regular text
	const int TITLE_FONT_SIZE = 60; //Font size of bigger text
	const int HIGH_SCORE_STORE = 0; //The slot in which the best score is stored
	unsigned int CURRENT_HIGH_SCORE = 0; //Current value of the best score
	const unsigned int MAX_SCORE = 1000000000; //Max posible score (so the value doesn't flip over and restart counting from 0)
}

