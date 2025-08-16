#define SDL_MAIN_USE_CALLBACKS

#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include "application.hpp"

// The block needs to be made a class
// It will have graphical and symbolic elements which may need
// to be made their own class or structs
struct SimpleBlock {
  int x, y;
  int width, height;
};

struct AppState {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Application* application = nullptr;
	
	std::vector<SimpleBlock> blocks;
	const int blockWidth = 100;
	const int blockHeight = 66;
};

extern "C" {
// Runs at startup - initialization
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	auto* state = new AppState();
	
	*appstate = state;

	state->window = SDL_CreateWindow(
		"Control Prototype",
		960, 720,
		SDL_WINDOW_RESIZABLE);
	if (!state->window) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
		return SDL_APP_FAILURE;
	}
	state->renderer = SDL_CreateRenderer(state->window, nullptr);
	if (!state->renderer) {
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
		return SDL_APP_FAILURE;
	}
	state->application = new Application(state->window, state->renderer);

	/*
	// Setup Dear ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForSDLRenderer(state->window, state->renderer);
	ImGui_ImplSDLRenderer3_Init(state->renderer);
	*/

	return SDL_APP_CONTINUE;
}

// Runs when a new event occurs - 'callback' or 'interrupt'
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	auto* state = static_cast<AppState*>(appstate);
	
	// Pass events to ImGui
	ImGui_ImplSDL3_ProcessEvent(event);
	
	state->application->callback(event);
	switch (event->type) {
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;

		case SDL_EVENT_MOUSE_BUTTON_DOWN: {
			// Only add block if ImGui has not captured click
			if (!ImGui::GetIO().WantCaptureMouse) {
				int x = event->button.x - state->blockWidth / 2;
				int y = event->button.y - state->blockHeight / 2;
				state->blocks.push_back({x, y, state->blockWidth, state->blockHeight});
			}
			break;
		}

		default:
			break;
	}

	return SDL_APP_CONTINUE;
}

// Runs each frame - main loop
SDL_AppResult SDL_AppIterate(void* appstate) {
	auto* state = static_cast<AppState*>(appstate);
	
	/*
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
	  state->blocks.clear();
	}
	ImGui::End();
	*/

	/*
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
	SDL_RenderClear(state->renderer);

	SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 128);
	for (const auto& blk : state->blocks) {
		SDL_FRect rect = {
			static_cast<float>(blk.x),
			static_cast<float>(blk.y),
			static_cast<float>(blk.width),
			static_cast<float>(blk.height)};
		SDL_RenderFillRect(state->renderer, &rect);
	}
	// Render ImGui    
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), state->renderer);

	SDL_RenderPresent(state->renderer);
	*/
	state->application->loop();

	SDL_Delay(16); // ~60 FPS

	return SDL_APP_CONTINUE;
}

// Runs at shutdown
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	auto* state = static_cast<AppState*>(appstate);

	(void)result;

	/*
	// Shutdown ImGui
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
	*/
	delete state->application;
	if (state->renderer) {
		SDL_DestroyRenderer(state->renderer);
	}
	if (state->window) {
		SDL_DestroyWindow(state->window);
	}
	SDL_Quit();
	delete state;
}

} // extern "C"

