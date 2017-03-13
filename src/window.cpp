#ifdef _WIN32
#include <windows.h>
#endif
#include <gl/gl.h>
#include <gl/glu.h>
#include <SDL.h>
#include <string>
#include "window.hpp"

Window :: Window(): m_window(NULL), m_screenSurface(NULL), m_GLContext(NULL), m_renderer(NULL) {}

Window* Window :: getInstance( )
{
	if ( !instance )	
		instance = new Window();

	return instance;
}

void Window :: openWindow( std::string title, int width, int height, bool openGL, Uint32 flags )
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
        if( m_window == NULL )
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

void Window :: openGLContext()
{
	m_GLContext = SDL_GL_CreateContext(m_window);

	if ( !m_GLContext )
		throw( std::runtime_error( std::string( "Failed to create GL context! SDL_Error: " ) + SDL_GetError() ) );
}

void Window :: dropGLContext()
{
	if (m_GLContext)
		SDL_GL_DeleteContext(m_GLContext); 
}

void Window :: swapBuffers()
{
	if ( m_GLContext )
		SDL_GL_SwapWindow(m_window);
	else if ( m_renderer )
		SDL_RenderPresent(m_renderer);
	else
		throw( std::runtime_error( "Attempted to swap buffers without renderer" ) );
}

void Window :: closeWindow( )
{
	if ( m_window )
	{
		dropGLContext();
		SDL_DestroyWindow( m_window );
	}
}

Window ::  ~Window()
{
	closeWindow( );
}

Window* Window::instance = NULL;