#pragma once
#include "application.hpp"

class MainApplication : public Application {
	public:
		MainApplication(SDL_Window* w, SDL_Renderer* r) : Application(w, r) { }
		//void loop();
		//void callback(SDL_Event* event);
		void draw() override;
		void drawMenu() override;
		void initialize() override;
	protected:
};

/*
class Application {
	public:
		Application(SDL_Window* w, SDL_Renderer* r) : window(w), renderer(r) { initialize(); }
		virtual ~Application();
		virtual void loop();
		void callback(SDL_Event* event);
		virtual void draw();
		virtual void drawMenu();
		ImGuiContext* context;		
		virtual void initialize();
	protected:
		SDL_Window* window;
		SDL_Renderer* renderer;
};

class AlternateEditorApplication : public AlternateApplication {
	public:
		//AlternateEditorApplication() : AlternateEditorApplication(nullptr, nullptr) { }
		//AlternateEditorApplication(SDL_Window* w, SDL_Renderer* r) : window(w), renderer(r) { }
		AlternateEditorApplication() : AlternateApplication() { }
		AlternateEditorApplication(SDL_Window* w, SDL_Renderer* r) : AlternateApplication(w, r) { }
		~AlternateEditorApplication();
		SDL_AppResult loop() override;
		SDL_AppResult callback(SDL_Event* event) override;
		void draw() override;
		void drawMenu() override;
		SDL_AppResult initialize() override;
		//SDL_WindowID getWindowID();
	protected:
		cv::Mat image;
		SDL_Texture* texture;
		ImTextureID imguiTexture = 0;
		//SDL_Window* window;
		//SDL_Renderer* renderer;
		//const int DEFAULT_WIDTH = 960;
		//const int DEFAULT_HEIGHT = 720;
};
*/

class AlternateMainApplication : public AlternateApplication {
	public:
		AlternateMainApplication() : AlternateApplication() { }
		AlternateMainApplication(SDL_Window* w, SDL_Renderer* r) : AlternateApplication(w, r) { }
		~AlternateMainApplication();
		SDL_AppResult loop() override;
		SDL_AppResult callback(SDL_Event* event) override;
		void draw() override;
		void drawMenu() override;
		SDL_AppResult initialize() override;
	protected:
    	    ImGuiContext* context;
};
