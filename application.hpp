#pragma once
#include "graphics.hpp"
#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

// Renderer

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

// Application

class Application {
	public:
		Application(SDL_Window* w, SDL_Renderer* r) : window(w), renderer(r) { initialize(); }
		~Application();
		void loop();
		void callback(SDL_Event* event);
		void drawMenu();

	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		void initialize();
};

Application::~Application() {
	// Shutdown ImGui
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void Application::drawMenu() {
	// Start ImGui frame
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	
	// Example window
	ImGui::Begin("Drawing Menu");
	ImGui::Text("Application: Filter Structure Diagram");
	ImGui::Button("Branch");
	ImGui::Button("Delay");
	ImGui::Button("Multiplier");
	ImGui::Button("Sum");
	ImGui::Button("Node");
	if (ImGui::Button("Clear")) {
	  //state->blocks.clear();
	}
	ImGui::End();
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
	/*
	for (const auto& blk : state->blocks) {
		SDL_FRect rect = {
			static_cast<float>(blk.x),
			static_cast<float>(blk.y),
			static_cast<float>(blk.width),
			static_cast<float>(blk.height)};
		SDL_RenderFillRect(state->renderer, &rect);
	}
	*/
	// Render ImGui    
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

	SDL_RenderPresent(renderer);
}

void Application::initialize() {
	// Setup Dear ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
}

void Application::loop() {
	drawMenu();
}

void Application::callback(SDL_Event* event) {
	// Pass events to ImGui
	ImGui_ImplSDL3_ProcessEvent(event);
}


