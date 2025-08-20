#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "editorapplication.hpp"

// Example app state
struct AppState {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    AlternateEditorApplication *editorApplication;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    auto *state = new AppState();
    *appstate = state;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to init SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    state->window = SDL_CreateWindow("SDL3 + OpenCV", 960, 720, SDL_WINDOW_RESIZABLE);
    if (!state->window) {
        return SDL_APP_FAILURE;
    }
    state->renderer = SDL_CreateRenderer(state->window, nullptr);
    if (!state->renderer) {
        return SDL_APP_FAILURE;
    }
    state->editorApplication =  new AlternateEditorApplication(state->window, state->renderer);
    return state->editorApplication->initialize();
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; // exit cleanly
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    auto *state = static_cast<AppState *>(appstate);
    
    return state->editorApplication->loop();
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    auto *state = static_cast<AppState *>(appstate);

    delete state->editorApplication;
    if (state->renderer) {
        SDL_DestroyRenderer(state->renderer);
    }
    if (state->window) {
        SDL_DestroyWindow(state->window);
    }
    delete state;

    SDL_Quit();
}

