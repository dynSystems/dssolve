#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <string>
#include <stdexcept>
#include "kernel.hpp"

Kernel::Kernel(): m_window(nullptr), m_screenSurface(nullptr), m_GLContext(nullptr), m_renderer(nullptr) {}

Kernel* Kernel:: getInstance( )
{
	if ( !instance )	
		instance = new Kernel();

	return instance;
}

void Kernel:: openWindow( std::string title, int width, int height, bool openGL, Uint32 flags )
{
	if ( m_window )
		throw( "Window already created" );

	//Initialize SDL Video subsystem
    if( SDL_InitSubSystem( SDL_INIT_VIDEO ) < 0 )
		throw( std::runtime_error( std::string( "SDL failed to initialize video! SDL_Error: " ) + SDL_GetError() ) );
    else {
        //Create window
		m_width	 = width;
		m_height = height;

		if ( openGL )
			flags = flags | SDL_WINDOW_OPENGL;

        m_window = SDL_CreateWindow( "SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, flags );
        if ( m_window == NULL )
			throw( std::runtime_error( std::string( "SDL_CreateWindow failed! SDL_Error: " ) + SDL_GetError() ) );
        else
        {
            // Get window surface
            m_screenSurface = SDL_GetWindowSurface( m_window );

			// Clear window
			if ( openGL )
			{
				openGLContext();
				glClearColor(0,0,0,1);
				glClear(GL_COLOR_BUFFER_BIT);
			} else
			{
				m_renderer = SDL_CreateRenderer(m_window, -1, 0);
				SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
				SDL_RenderClear(m_renderer);
			}
            
            //Update the surface
            swapBuffers();
        }
	}
}

void Kernel:: openGLContext()
{
	m_GLContext = SDL_GL_CreateContext(m_window);

	if ( !m_GLContext )
		throw( std::runtime_error( std::string( "Failed to create GL context! SDL_Error: " ) + SDL_GetError() ) );
}

void Kernel:: dropGLContext()
{
	if (m_GLContext)
		SDL_GL_DeleteContext(m_GLContext); 
}

void Kernel::eventLoop()
{
	bool done = false;
	SDL_Event event;
	while ((!done) && (SDL_WaitEvent(&event))) {
		switch (event.type) {
		case SDL_KEYDOWN:
		{
			SDL_Keycode key = event.key.keysym.sym;
			if (key == SDLK_ESCAPE)
				done = true;
			break;
		}

		case SDL_QUIT:
			done = true;
			break;
		}

		swapBuffers();
	}
}

void Kernel :: swapBuffers()
{
	if ( m_GLContext )
		SDL_GL_SwapWindow(m_window);
	else if ( m_renderer )
		SDL_RenderPresent(m_renderer);
	else
		throw( std::runtime_error( "Attempted to swap buffers without renderer" ) );
}

void Kernel :: closeWindow( )
{
	if ( m_window )
	{
		dropGLContext();
		SDL_DestroyWindow( m_window );
	}
}

Kernel ::  ~Kernel()
{
	closeWindow( );
}

Kernel* Kernel::instance = nullptr;
