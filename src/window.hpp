#include <string>
#include <SDL.h>

class Window
{
	private:
		static Window*	instance;
		SDL_Window*		m_window;
		SDL_Surface*	m_screenSurface;
		SDL_GLContext	m_GLContext;
		SDL_Renderer*	m_renderer;

		int				m_width;
		int				m_height;

		Window();

	public:
		Window(Window const&)         = delete;
		void operator=(Window const&) = delete;

		void openWindow(std::string title = "Window", int width = 640, int height = 480, bool openGL = true, Uint32 flags=SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
		void closeWindow();
		void openGLContext();
		void dropGLContext();
		void swapBuffers();

		static Window* getInstance( );
		~Window();
};

