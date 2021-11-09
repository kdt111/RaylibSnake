#pragma once

#include "iupdateable.h"
#include "idraweable.h"
#include "raylib.h"

#define MAIN_MENU_OPTION_COUNT 4
class MainMenu : public IDraweable, IUpdateable
{
private:
	const unsigned int menuOptionsOffset = 10;
	const char* menuOptions[MAIN_MENU_OPTION_COUNT] = {
		"New Game",
		"Options",
		"Leaderboard",
		"Exit"
	};
	const char* selectedOptionFormat = "> %s <";
	unsigned int currentMainMenuOption = 0;
public:
	MainMenu();
	~MainMenu();
	void Draw(float deltaTime) const override;
	void Update(float deltaTime) override;
};

