#pragma once
#include <math.h>

#include "config.h"
#include "raylib.h"

//Start menu
namespace MainMenu
{
	//Split the text into two lines to center them easier
	const char* START_GAME_TEXTS[] = {
		"PRESS [SPACE]",
		"TO START THE GAME"
	};

	const char* TITLE = "RAYLIB SNAKE";
	const char* HIGH_SCORE_TEXT = "Best score: %i";
	const char* FOOTNOTE = "Created by Jan Malek";

	void Draw()
	{
		//Calculate the offset from the egde of the screen, so that the text is centered
		int xPos = (Config::GAME_SIZE - MeasureText(TITLE, Config::TITLE_FONT_SIZE)) * 0.5f;
		DrawText(TITLE, xPos, 100, Config::TITLE_FONT_SIZE, BLACK);

		const char* highScore = TextFormat(HIGH_SCORE_TEXT, Config::CURRENT_HIGH_SCORE);
		xPos = (Config::GAME_SIZE - MeasureText(highScore, Config::FONT_SIZE)) * 0.5f;
		DrawText(highScore, xPos, 100 + Config::TITLE_FONT_SIZE + 10, Config::FONT_SIZE, BLACK);
		//Flash the text (1s on, 1s off)
		if (sin(GetTime() * 2 * PI) > 0)
		{
			for (int i = 0; i < 2; i++)
			{
				int xPos = (Config::GAME_SIZE - MeasureText(START_GAME_TEXTS[i], Config::FONT_SIZE)) * 0.5f;
				int yPos = (Config::GAME_SIZE - Config::FONT_SIZE * 2) * 0.5f + i * Config::FONT_SIZE + 5;
				DrawText(START_GAME_TEXTS[i], xPos, yPos, Config::FONT_SIZE, BLACK);
			}
		}
		xPos = (Config::GAME_SIZE - MeasureText(FOOTNOTE, Config::FONT_SIZE)) * 0.5f;
		DrawText(FOOTNOTE, xPos, Config::GAME_SIZE - Config::FONT_SIZE - 20, Config::FONT_SIZE, BLACK);
	}
}