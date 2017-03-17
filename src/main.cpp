#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#if _WIN32
#   include <Windows.h>
#endif
#include <SDL.h>
#include "glad/glad.h"
#include "kernel.hpp"

int main( int argc, char* args[] )
{   
	SDL_Init(0);
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		std::cout << std::string("SDL failed to initialize video! SDL_Error: ") + SDL_GetError() << std::endl;
		return -1;
	}

	Kernel* wnd = Kernel::getInstance();
	
	wnd->openWindow( "DSSolve", 640, 480, true );
	wnd->startGui();
	wnd->eventLoop();

	return 0;
}
