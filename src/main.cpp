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
// #include <ginac/ginac.h>
// #include <giac/giac.h>
#include "symbolicc++.h"
#include "window.hpp"

int main( int argc, char* args[] )
{   
// 	SDL_Init(0);
//
// 	Window* wnd = Window::getInstance();
// 	wnd->openWindow( "OpenDE", 640, 480, true );
//
// 	// Event loop here plz :D
// 	float dummy;
// 	std::cin >> dummy;
//
// 	delete wnd;
// 	SDL_Quit();

// 	std::vector<GiNaC::symbol> syms;
// 	syms.push_back(GiNaC::symbol("x"));
// 	syms.push_back(GiNaC::symbol("y"));
//
// 	GiNaC::parser p();
// 	GiNaC::ex MyEx2 = syms[0] + 1./2*pow(syms[1],2);
// 	std::cout << MyEx2 << " -> " << MyEx2.diff(syms[1], 1) << std::endl;
//
// 	const std::string expression = "2*cos(x)+exp(x)+2*y+3*z";
// 	GiNaC::parser p;
// 	GiNaC::ex e = p(expression);
// 	const GiNaC::symtab table = p.get_syms();
//
// 	for(auto s : table) std::cout << s.first << ": " << s.second << std::endl;
//
// 	// Get all derivatives
// 	std::vector<GiNaC::symbol> syms;
// 	for(auto s : table)	syms.push_back(GiNaC::symbol(s.first));
//
// 	GiNaC::parser pp(table);
// 	GiNaC::ex ee = pp(expression);
//
// 	for(const auto& s : syms) std::cout << ee.diff(s, 1) << std::endl;

// 	giac::gen y("y", nullptr);
// 	giac::gen e(std::string("sin(x)^2-4*cos(y)"), nullptr);
// 	std::cout << giac::_integrate(giac::makevecteur(e,y),nullptr);
// 	giac::parsed_gen(e, nullptr);


	// Hello world in symbols.
	Symbolic X("X"), XI("XI");

	std::cout << df(X, XI) << std::endl;
	std::cout << df(X[XI], XI) << std::endl;
	std::cout << df((2*X)^2, X) << std::endl;
	std::cout << df(cos(2*X) - sin(3*X), X) << std::endl;

	return 0;
}
