#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "../BasedEngine/Engine.h"
#include <Windows.h>
#include "VideoReader/video_reader.h"
#include <GLFW/glfw3.h>

int main()
{

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, 0);

	Engine engine;
	engine.init();

	while (!engine.isQuiting())
	{
		engine.load();
		while (!engine.isRestarting())
		{
			engine.updateTime();
			engine.update();// need to place in time
		}
		engine.unload();
	}
	engine.release();
}