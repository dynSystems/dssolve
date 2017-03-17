#pragma once

#include <string>
#if _WIN32
#   include <Windows.h>
#endif
#include <SDL.h>
#include "glad/glad.h"
#include "gui.hpp"

class Kernel
{
	private:
		static Kernel*	instance;
		SDL_Window*		m_window;
		SDL_Surface*	m_screenSurface;
		SDL_GLContext	m_GLContext;
		SDL_Renderer*	m_renderer;

		Gui*			m_Gui;

		int				m_width;
		int				m_height;

		Kernel();
		void drawGui();

	public:
		Kernel(Kernel const&)         = delete;
		void operator=(Kernel const&) = delete;

		void openWindow(std::string title = "Window", int width = 640, int height = 480, bool openGL = true, Uint32 flags=SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
		void startGui();
		void closeWindow();
		void openGLContext();
		void dropGLContext();
		void eventLoop();
		void swapBuffers();

		static Kernel* getInstance( );
		~Kernel();
};

