#define SDL_MAIN_USE_CALLBACKS

#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include "application.hpp"

struct AppState {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	MainApplication* application = nullptr;
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
	state->application = new MainApplication(state->window, state->renderer);

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
				//int x = event->button.x - state->blockWidth / 2;
				//int y = event->button.y - state->blockHeight / 2;
				//state->blocks.push_back({x, y, state->blockWidth, state->blockHeight});
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

	//state->application->loop();
	state->application->draw();

	SDL_Delay(16); // ~60 FPS

	return SDL_APP_CONTINUE;
}

// Runs at shutdown
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	auto* state = static_cast<AppState*>(appstate);

	(void)result;
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

