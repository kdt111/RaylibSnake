#pragma once

#include <vector>
#include "raylib.h"
#include "iupdateable.h"
#include "idraweable.h"

#ifndef WINDOW_NAME
#define WINDOW_NAME "Sample window"
#endif

namespace Engine
{
	//Window constants
	const unsigned int GAME_BOARD_SIZE = 800;
	const unsigned int GAME_CELLS = GAME_BOARD_SIZE / 40;
	const Color CLEAR_COLOR = WHITE;

	//Font constants
	const unsigned int DEF_FONT_SIZE = 30;
	const unsigned int TITLE_FONT_SIZE = 60;
	
	//GameState
	enum GameState{MainMenu, InGame, Paused};
	static GameState currentGameState;

	static std::vector<IUpdateable*> objToUpdate;
	static std::vector<IDraweable*> objToDraw;

	//Exit flags
	static bool shouldExit = false;

	//Returns the current GameState
	GameState GetCurrentGameState()
	{
		return currentGameState;
	}
	void SetCurrentGameState(GameState newState)
	{
		currentGameState = newState;
	}

	//Update loops
	void Update()
	{
		float delta = GetFrameTime();
		for (size_t i = 0; i < objToUpdate.size(); ++i)
			objToUpdate[i]->Update(delta);
	}
	void Draw()
	{
		float delta = GetFrameTime();
		for (size_t i = 0; i < objToDraw.size(); ++i)
			objToDraw[i]->Draw(delta);
	}

	//Additions and removal to the update and draw loops
	void RegisterToUpdate(IUpdateable* updateable)
	{
		for (size_t i = 0; i < objToUpdate.size(); i++)
			if(objToUpdate[i] == updateable)
				return;
		
		objToUpdate.push_back(updateable);
	}
	void RegisterToDraw(IDraweable* draweable)
	{
		for (size_t i = 0; i < objToDraw.size(); i++)
			if (objToDraw[i] == draweable)
				return;
		
		objToDraw.push_back(draweable);
	}

	void UnregisterFormUpdate(IUpdateable* updateable)
	{
		for (size_t i = 0; i < objToUpdate.size(); i++)
			if (objToUpdate[i] == updateable)
			{
				objToUpdate.erase(objToUpdate.begin() + i);
				return;
			}
	}
	void UnregisterFormDraw(IDraweable* draweable)
	{
		for (size_t i = 0; i < objToDraw.size(); i++)
			if (objToDraw[i] == draweable)
			{
				objToDraw.erase(objToDraw.begin() + i);
				return;
			}
	}

	int GetTextCenterX(const char* text, unsigned int fontSize)
	{
		return GetScreenWidth() / 2 - (float)MeasureText(text, fontSize) / 2;
	}

	void Exit()
	{
		shouldExit = true;	
	}
	bool ShouldExit()
	{
		return shouldExit;
	}
}
