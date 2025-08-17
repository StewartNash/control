#pragma once
#include "application.hpp"

class MainApplication : public Application {
	public:
		MainApplication(SDL_Window* w, SDL_Renderer* r) : Application(w, r) { }
		//void loop();
		//void callback(SDL_Event* event);
		void draw();
		void drawMenu();
};
