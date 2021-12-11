#pragma once

//Base game engine
namespace Engine
{
	extern const int GAME_SIZE; 			// Both vertical and horizontal size of the screen
	extern const int CELL_SIZE; 			// How many pixels does each cell of the game take
	extern const int FONT_SIZE;				// Font size of regular text
	extern const int TITLE_FONT_SIZE;		// Font size of bigger text
	extern const int TARGET_FRAMERATE;		// Framerate of the game

	// Initializes the engine and the window
	void Init();
	// Closes the engine and the window
	void Close();
	// Should the engine be closed
	bool ShouldClose();
	// Main engine update loop
	void Update();
	// Main engine draw loop
	void Draw();
	// Engine functions focused on the scoring system
	namespace Scoring
	{
		// Returns the best recorded score
		int GetBestScore();
		// Attempts to set the current score as the new best score
		void SetNewBestScore();
		// Returns the current score of the player
		int GetCurretScore();
		// Adds the passed amount to the current score
		void AddToCurrentScore(unsigned int amount);
		// Resets the current score back to 0
		void ResetCurrentScore();
	}

	//Engine functions for controlling the game state
	namespace GameState
	{
		enum GameStateType{IN_MENU, PLAYING};

		GameStateType GetCurrentGameState();
		void SetGameState(GameStateType newGameState);
	}
}

