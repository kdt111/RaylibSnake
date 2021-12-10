#pragma once

//Subset of functionalities for controlling the game
namespace Game
{
	//Initializes the game. Has to be called every time the game is started
	void Init();
	//Updates the game loop
	void Update();
	//Draws the current game state
	void Draw();
}