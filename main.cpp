#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <opencv2/opencv.hpp>
#include <iostream>


// Example app state
struct AppState {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *texture = nullptr;
    cv::Mat image;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    auto *state = new AppState();
    *appstate = state;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to init SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    state->window = SDL_CreateWindow("SDL3 + OpenCV", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!state->window) return SDL_APP_FAILURE;

    state->renderer = SDL_CreateRenderer(state->window, nullptr);
    if (!state->renderer) return SDL_APP_FAILURE;

    // Load image with OpenCV
    state->image = cv::imread("../data/img531.png");
    if (state->image.empty()) {
        SDL_Log("Failed to load image with OpenCV");
        return SDL_APP_FAILURE;
    }
    cv::cvtColor(state->image, state->image, cv::COLOR_BGR2RGB);

    state->texture = SDL_CreateTexture(
        state->renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STATIC,
        state->image.cols,
        state->image.rows
    );
    if (!state->texture) return SDL_APP_FAILURE;

    SDL_UpdateTexture(state->texture, nullptr, state->image.data, state->image.step);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; // exit cleanly
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    auto *state = static_cast<AppState *>(appstate);

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    if (state->texture) {
        SDL_RenderTexture(state->renderer, state->texture, nullptr, nullptr);
    }

    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    auto *state = static_cast<AppState *>(appstate);

    if (state->texture) SDL_DestroyTexture(state->texture);
    if (state->renderer) SDL_DestroyRenderer(state->renderer);
    if (state->window) SDL_DestroyWindow(state->window);

    delete state;

    SDL_Quit();
}

