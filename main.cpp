//Standard headers
#include <math.h>
#include <time.h>

//Required headers
#include "raylib.h"
#include "config.h"

//Implementation files
#include "impl/mainmenu.cpp"
#include "impl/game.cpp"

//Window title
#ifdef DEBUG
#define WINDOW_TITLE "RaylibSnake (DEBUG)"
#else
#define WINDOW_TITLE "RaylibSnake"
#endif

const Vector2 Vector2ZERO = {0.0f, 0.0f};

void UpdateLoop();
void DrawLoop(RenderTexture2D &renderTexture);

int main(void)
{
	//Window creation and configuration
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(Config::GAME_SIZE, Config::GAME_SIZE, WINDOW_TITLE);
	SetTargetFPS(Config::TARGET_FRAMERATE);
	SetRandomSeed(time(NULL));
	SetWindowMinSize(Config::GAME_SIZE, Config::GAME_SIZE);

	Config::CURRENT_HIGH_SCORE = LoadStorageValue(Config::HIGH_SCORE_STORE);

	//Render texture will be used to scale the game to the window size
	//That way the game board size will be constant and displayed on every resolution
	RenderTexture2D renderTexture = LoadRenderTexture(Config::GAME_SIZE, Config::GAME_SIZE);
	SetTextureFilter(renderTexture.texture, TEXTURE_FILTER_BILINEAR);

	//Main game loop
	while(true)
	{
		UpdateLoop();
		DrawLoop(renderTexture);
		
		//Only close the windows when the 'X' is pressed
		if((WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE))) break;
	}

	//Save the high score
	SaveStorageValue(Config::HIGH_SCORE_STORE, Config::CURRENT_HIGH_SCORE);

	//Clean up
	UnloadRenderTexture(renderTexture);
	CloseWindow();
}

//Updates the game state
void UpdateLoop()
{
	if (Config::PLAYING)
		Game::Update();
	else
	{
		if (IsKeyPressed(KEY_SPACE))
		{
			Game::Init();
			Config::PLAYING = true;
		}
	}
}

//Draws the latest game state
void DrawLoop(RenderTexture2D &renderTexture)
{
	//Rendering the game to a render texture
	BeginTextureMode(renderTexture);
	ClearBackground(Config::CLEAR_COLOR);
	if (Config::PLAYING)
		Game::Draw();
	else
		MainMenu::Draw();
	EndTextureMode();

	//Rendering the scaled render texture to the screen
	float screenWidth = GetScreenWidth();
	float screenHeight = GetScreenHeight();
	float renderScale = fminf(screenWidth / Config::GAME_SIZE, screenHeight / Config::GAME_SIZE);
	float gameSizeScaled = Config::GAME_SIZE * renderScale;

	static Rectangle scr = {0.0f, 0.0f, Config::GAME_SIZE, -(float)Config::GAME_SIZE}; //Have to flip the the render texture on the Y axis for OpenGL reasons...
	Rectangle dest = {(screenWidth - gameSizeScaled) * 0.5f, (screenHeight - gameSizeScaled) * 0.5f, gameSizeScaled, gameSizeScaled};

	BeginDrawing();
	ClearBackground(BLACK);
	DrawTexturePro(renderTexture.texture, scr, dest, Vector2ZERO, 0.0f, WHITE);
	EndDrawing();
}