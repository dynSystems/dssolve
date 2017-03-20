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
#include "SymbolicCpp\symbolicc++.h"

int main( int argc, char* args[] )
{   
	SDL_Init(0);
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		std::cout << std::string("SDL failed to initialize video! SDL_Error: ") + SDL_GetError() << std::endl;
		return -1;
	}

	Kernel* wnd = Kernel::getInstance();
	
	Symbolic x("x");
	cout << integrate(x + 1, x);     // => 1/2*x^(2)+x
	Symbolic y("y");
	cout << df(y, x);              // => 0
	cout << df(y[x], x);           // => df(y[x],x)

	wnd->openWindow( "DSSolve", 640, 480, true );
	wnd->startGui();
	wnd->eventLoop();

	return 0;
}
