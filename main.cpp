#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
//#include <opencv2/opencv.hpp>
//#include <iostream>

#include "application.hpp"

// Example app state
struct AppState {
    MenuApplication *application;
};

static inline SDL_WindowID eventWindowID(const SDL_Event& e);

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    auto *state = new AppState();
    *appstate = state;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to init SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    state->application =  new MenuApplication();

    return state->application->initialize();
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    auto *state = static_cast<AppState *>(appstate);
    const SDL_WindowID windowID = eventWindowID(*event);
    
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; // exit cleanly
    }
    
    if (windowID) {
            /*
	    if (state->mainWindow && windowID == SDL_GetWindowID(state->mainWindow)) {
		    state->application->callback(event);
	    } else if (state->editorWindow && event->window.windowID == SDL_GetWindowID(state->editorWindow)) {
		    state->editorApplication->callback(event);
	    }
	    */
	    if (windowID == state->application->getWindowID()) {
	        return state->application->callback(event);
	    }
    }
    
    switch (event->type) {
	    case SDL_EVENT_QUIT:
	    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
		    return SDL_APP_SUCCESS;
	    default:
		    break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    auto *state = static_cast<AppState *>(appstate);
    
    return state->application->loop();
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    auto *state = static_cast<AppState *>(appstate);

    delete state->application;
    delete state;

    SDL_Quit();
}

static inline SDL_WindowID eventWindowID(const SDL_Event& e) {
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

