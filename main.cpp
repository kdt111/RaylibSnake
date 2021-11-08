#include "include/raylib.h"
#include "include/engine.h"

#ifndef WINDOW_NAME
#define WINDOW_NAME "Sample window"
#endif

void UpdateLoop();
void DrawLoop();

int main(void)
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(Engine::GAME_BOARD_SIZE, Engine::GAME_BOARD_SIZE, WINDOW_NAME);

	HideCursor();

	while(true)
	{
		UpdateLoop();
		DrawLoop();
		
		if(WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE)) break;
	}

	CloseWindow();
}

void UpdateLoop()
{

}

void DrawLoop()
{
	BeginDrawing();
	ClearBackground(Engine::CLEAR_COLOR);

	switch (Engine::GetCurrentGameState())
	{
		case Engine::GameState::MainMenu:
			DrawText("MainMenu", 20, 20, Engine::DEF_FONT_SIZE, BLACK);
			break;
		case Engine::GameState::InGame:
			DrawText("InGame", 20, 20, Engine::DEF_FONT_SIZE, BLACK);
			break;
		case Engine::GameState::Paused:
			DrawText("Paused", 20, 20, Engine::DEF_FONT_SIZE, BLACK);
			break;
	}

	EndDrawing();
}
