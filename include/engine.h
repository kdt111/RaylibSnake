#pragma once
#include "raylib.h"

namespace Engine
{
	//Window constants
	const unsigned int GAME_BOARD_SIZE = 800;
	const Color CLEAR_COLOR = WHITE;

	//Font constants
	const unsigned int DEF_FONT_SIZE = 30;
	
	enum GameState{MainMenu, InGame, Paused};
	static GameState currentGameState;

	GameState GetCurrentGameState()
	{
		return currentGameState;
	}

	void SetCurrentGameState(GameState newState)
	{
		currentGameState = newState;
	}
}
