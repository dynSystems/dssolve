#pragma once

#if _WIN32
#   include <Windows.h>
#endif
#include <SDL.h>
#include "glad/glad.h"
#include "nanogui/nanogui.h"
#include <map>
#include <iostream>

class Gui : public nanogui::Screen
{
	private:
		std::map<std::string, nanogui::ref<nanogui::Window>> windows;

	public:
		Gui( SDL_Window* window, int width, int height );
		~Gui() {};

		Gui(Gui const&) = delete;
		void operator=(Gui const&) = delete;
		
		nanogui::ref<nanogui::Window>	addWindow( std::string, std::string title = "" );
		nanogui::ref<nanogui::Window>	getWindow( std::string );

		void draw();
		void passEvents(SDL_Event& e);

		virtual void draw(NVGcontext *ctx) {
			Screen::draw(ctx);
		}
		virtual void drawContents()
		{
		}
};