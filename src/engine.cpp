#include <raylib.h>
#include <raymath.h>
#include <time.h>

#include "engine.h"
#include "mainmenu.h"
#include "game.h"

namespace Engine
{
	const int GAME_SIZE = 800;
	const int CELL_SIZE = 20;
	const int FONT_SIZE = 30;
	const int TITLE_FONT_SIZE = 60;

	static int targetFramerate = 15;
	static RenderTexture2D target;
	static Color clearColor = WHITE;

	namespace GameState
	{
		static GameStateType currentState = GameStateType::IN_MENU;

		GameStateType GetCurrentGameState()
		{
			return currentState;
		}

		void SetGameState(GameStateType newGameState)
		{
			switch (newGameState)
			{
			case IN_MENU:
				break;
			case PLAYING:
				Game::Init();
				break;
			}
			currentState = newGameState;
		}

		static void UpdateCurrentGameState()
		{
			switch (currentState)
			{
			case IN_MENU:
				MainMenu::Update();
				break;
			case PLAYING:
				Game::Update();
				break;
			}
		}

		static void DrawCurrentGameState()
		{
			switch (currentState)
			{
			case IN_MENU:
				MainMenu::Draw();
				break;
			case PLAYING:
				Game::Draw();
				break;
			}
		}
	}

	namespace Scoring
	{
		static int highScoreStore = 0;
		static unsigned int currentScore;
		static unsigned int bestScore;
		static unsigned int maxScore = 1000000000;

		int GetBestScore()
		{
			return bestScore;
		}

		void SetNewBestScore()
		{
			if (currentScore > bestScore)
			{
				bestScore = currentScore;
				SaveStorageValue(highScoreStore, bestScore);
			}
		}

		int GetCurretScore()
		{
			return currentScore;
		}

		void AddToCurrentScore(unsigned int amount)
		{
			if (currentScore + amount <= maxScore)
				currentScore += amount;
		}

		void ResetCurrentScore()
		{
			currentScore = 0;
		}
	}

	void Init()
	{
		#ifdef DEBUG
		const char* windowTitle = "RaylibSnake (DEBUG)";
		#else
		const char* windowTitle = "RaylibSnake";
		SetTraceLogLevel(LOG_NONE);
		#endif

		// Window creation and configuration
		SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
		InitWindow(GAME_SIZE, GAME_SIZE, windowTitle);
		SetTargetFPS(targetFramerate);
		SetRandomSeed(time(0));
		SetWindowMinSize(GAME_SIZE, GAME_SIZE);

		// Initialize scoring
		Scoring::bestScore = LoadStorageValue(Scoring::highScoreStore);
		Scoring::ResetCurrentScore();

		// Render texture will be used to scale the game to the window size
		target = LoadRenderTexture(GAME_SIZE, GAME_SIZE);
		SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
	}

	void Close()
	{
		//Unload used resources and close the window
		UnloadRenderTexture(target);
		CloseWindow();
	}

	bool ShouldClose()
	{
		return WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE);
	}

	void Update()
	{
		//Update the game based on it's current state
		GameState::UpdateCurrentGameState();
	}

	void Draw()
	{
		// Rendering the game to a render texture
		BeginTextureMode(target);
		ClearBackground(clearColor);
		GameState::DrawCurrentGameState();
		EndTextureMode();

		// Rendering the scaled render texture to the screen
		float screenWidth = GetScreenWidth();
		float screenHeight = GetScreenHeight();
		float renderScale = fminf(screenWidth / Engine::GAME_SIZE, screenHeight / Engine::GAME_SIZE);
		float gameSizeScaled = Engine::GAME_SIZE * renderScale;

		static Rectangle scr = {0.0f, 0.0f, Engine::GAME_SIZE, -(float)Engine::GAME_SIZE}; // Have to flip the the render texture on the Y axis for OpenGL reasons...
		Rectangle dest = {(screenWidth - gameSizeScaled) * 0.5f, (screenHeight - gameSizeScaled) * 0.5f, gameSizeScaled, gameSizeScaled};

		BeginDrawing();
		ClearBackground(BLACK);
		DrawTexturePro(target.texture, scr, dest, Vector2Zero(), 0.0f, WHITE);
		EndDrawing();
	}
}