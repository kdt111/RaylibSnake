//Standard headers
#include <memory>

//Required headers
#include "include/raylib.h"
#include "include/engine.h"

//Implementation files
#include "impl/mainmenu.cpp"


void UpdateLoop();
void DrawLoop();

int main(void)
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(Engine::GAME_BOARD_SIZE, Engine::GAME_BOARD_SIZE, WINDOW_NAME);

	HideCursor();

	std::unique_ptr<MainMenu> mainMenu = std::make_unique<MainMenu>();

	while(true)
	{
		UpdateLoop();
		DrawLoop();
		
		if((WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE)) || Engine::ShouldExit()) break;
	}

	CloseWindow();
}

void UpdateLoop()
{
	Engine::Update();
}

void DrawLoop()
{
	BeginDrawing();
	ClearBackground(Engine::CLEAR_COLOR);

	Engine::Draw();

	DrawFPS(20, 20);
	EndDrawing();
}
