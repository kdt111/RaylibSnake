// Game headers
#include "engine.h"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

void UpdateDrawLoop();

int main(void)
{
	Engine::Init();
	#ifdef PLATFORM_WEB
	emscripten_set_main_loop(UpdateDrawLoop, Engine::TARGET_FRAMERATE, 1);
	#else
	// Main game loop
	while(!Engine::ShouldClose())
	{
		UpdateDrawLoop();
	}
	#endif

	Engine::Close();
}

void UpdateDrawLoop()
{
	Engine::Update();
	Engine::Draw();
}