#include <raylib.h>
#include <math.h>

#include "engine.h"

namespace MainMenu
{
	// Split the text into two lines to center them easier
	const char *startGameTexts[] = {
		"PRESS [SPACE]",
		"TO START THE GAME"};

	const char *gameTitle = "RAYLIB SNAKE";
	const char *bestScoreText = "Best score: %i";
	const char *creatorInfo = "Created by Jan Malek";

	void Update()
	{
		// When Space is pressed, change the game state to playing
		if(IsKeyPressed(KEY_SPACE))
			Engine::GameState::SetGameState(Engine::GameState::PLAYING);
	}

	void Draw()
	{
		// Calculate the offset from the egde of the screen, so that the text is centered
		int xPos = (Engine::GAME_SIZE - MeasureText(gameTitle, Engine::TITLE_FONT_SIZE)) * 0.5f;
		DrawText(gameTitle, xPos, 100, Engine::TITLE_FONT_SIZE, BLACK);

		const char *highScore = TextFormat(bestScoreText, Engine::Scoring::GetBestScore());
		xPos = (Engine::GAME_SIZE - MeasureText(highScore, Engine::FONT_SIZE)) * 0.5f;
		DrawText(highScore, xPos, 100 + Engine::TITLE_FONT_SIZE + 10, Engine::FONT_SIZE, BLACK);
		
		// Flash the text (1s on, 1s off)
		if (sin(GetTime() * 2 * PI) > 0)
		{
			for (int i = 0; i < 2; i++)
			{
				int xPos = (Engine::GAME_SIZE - MeasureText(startGameTexts[i], Engine::FONT_SIZE)) * 0.5f;
				int yPos = (Engine::GAME_SIZE - Engine::FONT_SIZE * 2) * 0.5f + i * Engine::FONT_SIZE + 5;
				DrawText(startGameTexts[i], xPos, yPos, Engine::FONT_SIZE, BLACK);
			}
		}
		xPos = (Engine::GAME_SIZE - MeasureText(creatorInfo, Engine::FONT_SIZE)) * 0.5f;
		DrawText(creatorInfo, xPos, Engine::GAME_SIZE - Engine::FONT_SIZE - 20, Engine::FONT_SIZE, BLACK);
	}
}