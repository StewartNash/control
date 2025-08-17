#pragma once
#include "application.hpp"

class EditorApplication : public Application {
	public:
		EditorApplication(SDL_Window* w, SDL_Renderer* r) : Application(w, r) { }
		//void loop();
		//void callback(SDL_Event* event);
		void draw();
		void drawMenu();
};

