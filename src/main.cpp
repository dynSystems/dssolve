#include <stdio.h>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <gl/gl.h>
#include <gl/glu.h>
#include <SDL.h>
#include "window.hpp"

int main( int argc, char* args[] )
{   
	SDL_Init(0);

	Window* wnd = Window::getInstance();
	wnd->openWindow( "OpenDE", 640, 480, true );

	// Event loop here plz :D
	float dummy;
	std::cin >> dummy;

	delete wnd;
	SDL_Quit();

	return 0;
}