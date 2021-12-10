// Game headers
#include "engine.h"

int main(void)
{
	Engine::Init();

	// Main game loop
	while(!Engine::ShouldClose())
	{
		Engine::Update();
		Engine::Draw();
	}

	Engine::Close();
}