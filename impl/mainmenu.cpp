#pragma once

#include "../include/mainmenu.h"
#include "../include/engine.h"
#include "../include/raylib.h"

MainMenu::MainMenu()
{
	Engine::RegisterToUpdate(this);
	Engine::RegisterToDraw(this);
}

MainMenu::~MainMenu()
{
	Engine::UnregisterFormUpdate(this);
	Engine::UnregisterFormDraw(this);
}

void MainMenu::Draw(float deltaTime) const
{
	if (Engine::GetCurrentGameState() == Engine::GameState::MainMenu)
	{
		unsigned int baseOffset = Engine::TITLE_FONT_SIZE + 20 + MAIN_MENU_OPTION_COUNT * Engine::DEF_FONT_SIZE + (MAIN_MENU_OPTION_COUNT - 1) * menuOptionsOffset;
		DrawText(WINDOW_NAME, Engine::GetTextCenterX(WINDOW_NAME, Engine::TITLE_FONT_SIZE), baseOffset, Engine::TITLE_FONT_SIZE, GREEN);
		unsigned int currentOffset = Engine::TITLE_FONT_SIZE + baseOffset + 20;
		for (size_t i = 0; i < MAIN_MENU_OPTION_COUNT; i++)
		{
			const char* text = currentMainMenuOption == i ? TextFormat(selectedOptionFormat, menuOptions[i]) : menuOptions[i];
			DrawText(text, Engine::GetTextCenterX(text, Engine::DEF_FONT_SIZE), currentOffset, Engine::DEF_FONT_SIZE, BLACK);
			currentOffset += Engine::DEF_FONT_SIZE + menuOptionsOffset;
		}
	}
}

void MainMenu::Update(float deltaTime)
{
	if(Engine::GetCurrentGameState() == Engine::GameState::MainMenu)
	{
		if(IsKeyPressed(KEY_UP))
			currentMainMenuOption = (currentMainMenuOption - 1) % MAIN_MENU_OPTION_COUNT;
		else if(IsKeyPressed(KEY_DOWN))
			currentMainMenuOption = (currentMainMenuOption + 1) % MAIN_MENU_OPTION_COUNT;

		if(IsKeyPressed(KEY_ENTER))
		{
			if(currentMainMenuOption == 0) //New game
			{

			}
			else if(currentMainMenuOption == 1) //Options
			{

			}
			else if(currentMainMenuOption == 2) //Leaderboard
			{

			}
			else //Exit
			{
				Engine::Exit();
			}
		}
	}
}
