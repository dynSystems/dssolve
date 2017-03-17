#include <stdio.h>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include "window.hpp"

int main( int argc, char* args[] )
{   
	SDL_Init(0);
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		std::cout << std::string("SDL failed to initialize video! SDL_Error: ") + SDL_GetError() << std::endl;
		return -1;
	}

	Window* wnd = Window::getInstance();
	
	wnd->openWindow( "DSSolve", 640, 480, true );

	bool done = false;
	SDL_Event event;
	while ((!done) && (SDL_WaitEvent(&event))) {
		switch (event.type) {
			case SDL_KEYDOWN:
			{
				SDL_Keycode key = event.key.keysym.sym;
				if ( key == SDLK_ESCAPE )
					done = true;
				break;
			}

			case SDL_QUIT:
				done = true;
				break;
		}
	}

	delete wnd;
	SDL_Quit();

	return 0;
}