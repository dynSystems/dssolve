#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
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
	wnd->eventLoop();

	return 0;
}
