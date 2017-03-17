#include "gui.hpp"

Gui :: Gui( SDL_Window* window, int width, int height ) : nanogui::Screen(window, Eigen::Vector2i(width, height), "DSSolve")
{
}

void Gui :: draw()
{
	glViewport(0, 0, width(), height());
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	this->drawAll();
}

void Gui :: passEvents(SDL_Event& e)
{
	this->onEvent(e);
}

nanogui::ref<nanogui::Window> Gui::addWindow(std::string name, std::string title)
{
	if ( title.empty() )
		title = name;

	std::map<std::string, nanogui::ref<nanogui::Window>>::iterator it = windows.find(name);
	if ( it != windows.end() )
		it->second->dispose();

	nanogui::ref<nanogui::Window> wnd = new nanogui::Window(this, title);
	wnd->setPosition(Eigen::Vector2i(15, 15));
	wnd->setLayout(new nanogui::GroupLayout());
	windows[name] = wnd;

	/* For testing */
	nanogui::ref<nanogui::Button> b = wnd->add<nanogui::Button>("Styled", ENTYPO_ICON_ROCKET);
	b->setBackgroundColor(nanogui::Color(0, 0, 255, 25));
	b->setCallback([] { std::cout << "OHHHH GOD!" << std::endl; });
	b->setTooltip("I am the button which releases the hounds!");

	setVisible(true);
	performLayout(mNVGContext);
	return wnd;
}

nanogui::ref<nanogui::Window> Gui::getWindow(std::string name)
{
	std::map<std::string, nanogui::ref<nanogui::Window>>::iterator it = windows.find(name);
	if (it != windows.end())
		return it->second;
	else
		return nullptr;
}

