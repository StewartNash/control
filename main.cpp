#define SDL_MAIN_USE_CALLBACKS

#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include "application.hpp"
#include "mainapplication.hpp"
#include "editorapplication.hpp"

/*
enum ApplicationType {
	MAIN,
	EDITOR,
	TRAINER
};

struct ApplicationState {
	SDL_Window* window[3];
	SDL_Renderer* renderer[3];
	Application* application[3];
};
*/
struct ApplicationState {
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	SDL_Window* editorWindow;
	SDL_Renderer* editorRenderer;
	MainApplication* mainApplication;
	EditorApplication* editorApplication;
};

extern "C" {

static inline SDL_WindowID EventWindowID(const SDL_Event& e);

// Runs at startup - initialization
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	auto* state = new ApplicationState();
	
	*appstate = state;
	state->mainWindow = SDL_CreateWindow(
		"Control Prototype",
		960, 720,
		SDL_WINDOW_RESIZABLE);
	if (!state->mainWindow) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
		return SDL_APP_FAILURE;
	}
	state->mainRenderer = SDL_CreateRenderer(state->mainWindow, nullptr);
	if (!state->mainRenderer) {
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
		return SDL_APP_FAILURE;
	}
	state->mainApplication = new MainApplication(state->mainWindow, state->mainRenderer);

	state->editorWindow = SDL_CreateWindow(
		"Editor Prototype",
		960, 720,
		SDL_WINDOW_RESIZABLE);
	if (!state->editorWindow) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
		return SDL_APP_FAILURE;
	}
	state->editorRenderer = SDL_CreateRenderer(state->editorWindow, nullptr);
	if (!state->editorRenderer) {
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
		return SDL_APP_FAILURE;
	}
	state->editorApplication = new EditorApplication(state->editorWindow, state->editorRenderer);

	return SDL_APP_CONTINUE;
}

// Runs when a new event occurs - 'callback' or 'interrupt'
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	auto* state = static_cast<ApplicationState*>(appstate);
	const SDL_WindowID windowID = EventWindowID(*event);

	if (windowID) {
		if (state->mainWindow && windowID == SDL_GetWindowID(state->mainWindow)) {
			state->mainApplication->callback(event);
		} else if (state->editorWindow && event->window.windowID == SDL_GetWindowID(state->editorWindow)) {
			state->editorApplication->callback(event);
		}
	} else {
		/*
		if (state->mainApplication) {
			state->mainApplication->callback(event);
		}
		if (state->editorApplication) {
			state->editorApplication->callback(event);
		}
		*/
	}
	switch (event->type) {
		case SDL_EVENT_QUIT:
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			return SDL_APP_SUCCESS;
		/*
		case SDL_EVENT_MOUSE_BUTTON_DOWN: {
			// Only add block if ImGui has not captured click
			if (!ImGui::GetIO().WantCaptureMouse) {
				//int x = event->button.x - state->blockWidth / 2;
				//int y = event->button.y - state->blockHeight / 2;
				//state->blocks.push_back({x, y, state->blockWidth, state->blockHeight});
			}
			break;
		}
		*/
		default:
			break;
	}

	return SDL_APP_CONTINUE;
}

// Runs each frame - main loop
SDL_AppResult SDL_AppIterate(void* appstate) {
	auto* state = static_cast<ApplicationState*>(appstate);

	//state->application->loop();
	state->mainApplication->draw();
	state->editorApplication->draw();

	SDL_Delay(16); // ~60 FPS

	return SDL_APP_CONTINUE;
}

// Runs at shutdown
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	auto* state = static_cast<ApplicationState*>(appstate);

	(void)result;
	delete state->mainApplication;
	if (state->mainRenderer) {
		SDL_DestroyRenderer(state->mainRenderer);
	}
	if (state->mainWindow) {
		SDL_DestroyWindow(state->mainWindow);
	}	

	delete state->editorApplication;
	if (state->editorRenderer) {
		SDL_DestroyRenderer(state->editorRenderer);
	}
	if (state->editorWindow) {
		SDL_DestroyWindow(state->editorWindow);
	}
	SDL_Quit();
	delete state;
}

static inline SDL_WindowID EventWindowID(const SDL_Event& e) {
	switch (e.type) {
		case SDL_EVENT_WINDOW_SHOWN:
		case SDL_EVENT_WINDOW_HIDDEN:
		case SDL_EVENT_WINDOW_EXPOSED:
		case SDL_EVENT_WINDOW_MINIMIZED:
		case SDL_EVENT_WINDOW_MAXIMIZED:
		case SDL_EVENT_WINDOW_RESTORED:
		case SDL_EVENT_WINDOW_MOUSE_ENTER:
		case SDL_EVENT_WINDOW_MOUSE_LEAVE:
		case SDL_EVENT_WINDOW_FOCUS_GAINED:
		case SDL_EVENT_WINDOW_FOCUS_LOST:
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			return e.window.windowID;
		case SDL_EVENT_MOUSE_MOTION:
			return e.motion.windowID;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
			return e.button.windowID;
		case SDL_EVENT_MOUSE_WHEEL:
			return e.wheel.windowID;
		case SDL_EVENT_KEY_DOWN:
		case SDL_EVENT_KEY_UP:
			return e.key.windowID;
		case SDL_EVENT_TEXT_INPUT:
		case SDL_EVENT_TEXT_EDITING:
			return e.text.windowID;
		case SDL_EVENT_RENDER_TARGETS_RESET:
		case SDL_EVENT_RENDER_DEVICE_RESET:
			return e.render.windowID;
		default:
			return 0; // non-window-specific event
	}
}

} // extern "C"

