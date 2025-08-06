#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct SDLState {
	SDL_Window *window;
	SDL_Renderer *renderer;
};

void cleanup(SDLState &state);

int main(int argc, char* argv[]) {
	SDLState state;

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);

		return 1;
	}

	// Create window
	int width = 720;
	int height = 480;
	state.window = SDL_CreateWindow("SDL3 Demo", width, height, 0);		
	if (!state.window) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating window", state.window);
		cleanup(state);

		return 1;
	}

	// Create renderer
	state.renderer = SDL_CreateRenderer(state.window, nullptr);
	if (!state.renderer) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating renderer", state.window);
		cleanup(state);

		return 1;
	}

	// Start game loop
	bool running = true;
	while (running) {
		SDL_Event event{ 0 };
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_EVENT_QUIT:
					running = false;
					break;
			}
		}
		// Draw
		SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
		SDL_RenderClear(state.renderer);

		SDL_RenderPresent(state.renderer);
	}

	cleanup(state);

	return 0;
}

void cleanup(SDLState &state) {
	SDL_DestroyRenderer(state.renderer);
	SDL_DestroyWindow(state.window);
	SDL_Quit();
}
