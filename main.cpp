#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_FPoint points[500];

// Runs at startup - initialization
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
	int i;
  
	SDL_SetAppMetadata("Example Renderer Primitives", "1.0", "com.example.renderer-primitives");
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	if (!SDL_CreateWindowAndRenderer("examples/renderer/primitives", 640, 480, 0, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

// Runs when a new event occurs - 'callback' or 'interrupt'
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	return SDL_APP_CONTINUE;
}

// Runs each frame - main loop
SDL_AppResult SDL_AppIterate(void *appstate) {
	SDL_FRect rect;
	
	SDL_SetRenderDrawColor(renderer, 33, 33, 33, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
	rect.x = rect.y = 100;
	rect.w = 440;
	rect.h = 280;
	SDL_RenderFillRect(renderer, &rect);
	
	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

// Runs at shutdown
void SDL_AppQuit(void *appstate, SDL_AppResult result) {

}
