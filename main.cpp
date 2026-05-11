#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Engine* engine = new Engine;
	bool result;

	result = engine->Initialize();

	if (result)
	{
		engine->Run();
	}

	engine->Shutdown();
	delete engine;
	engine = nullptr;

	return 0;
}