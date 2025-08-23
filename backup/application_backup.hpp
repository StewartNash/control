#pragma once
#include "graphics.hpp"
#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

class SDLRenderer : public IRenderer {
	public:
		explicit SDLRenderer(SDL_Renderer* r) : renderer(r) {}
		
		void setDrawColor(int r, int g, int b, int a) override {
			SDL_SetRenderDrawColor(renderer, r, g, b, a);
		}
		void drawLine(double x1, double y1, double x2, double y2) override {
			SDL_RenderLine(renderer, (float)x1, (float)y1, (float)x2, (float)y2);
		}
		void fillRect(double x, double y, double w, double h) override {
			SDL_FRect rect { x, y, w, h };
			SDL_RenderFillRect(renderer, &rect);
		}
		void fillTriangle(double x, double y, double w, double h) override {
		
		}

	private:
		SDL_Renderer* renderer;
};

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

class AlternateApplication {
	public:
		AlternateApplication() : AlternateApplication(nullptr, nullptr) { }
		AlternateApplication(SDL_Window* w, SDL_Renderer* r) : window(w), renderer(r) { }
		virtual ~AlternateApplication();
		virtual SDL_AppResult loop();
		virtual SDL_AppResult callback(SDL_Event* event);
		virtual void draw();
		virtual void drawMenu();
		virtual SDL_AppResult initialize();
		SDL_WindowID getWindowID();
	protected:
		//cv::Mat image;
		//SDL_Texture* texture;
		//ImTextureID imguiTexture = 0;
		SDL_Window* window;
		SDL_Renderer* renderer;
		const int DEFAULT_WIDTH = 960;
		const int DEFAULT_HEIGHT = 720;
};

/*
class MenuApplication : public AlternateApplication {
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
*/
