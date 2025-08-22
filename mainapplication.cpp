#include "mainapplication.hpp"

void MainApplication::draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);

	if (ImGui::GetCurrentContext() != context) {
		ImGui::SetCurrentContext(context);
	}
        // Start ImGui frame
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	
	drawMenu();

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

void MainApplication::drawMenu() {
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
}

void MainApplication::initialize() {
	// Setup Dear ImGui
	//IMGUI_CHECKVERSION();
	context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);
	
	ImGui::StyleColorsDark();
	
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
}

//////////////////////////////
// AlternateMainApplication //
//////////////////////////////

AlternateMainApplication::~AlternateMainApplication() {
    if (ImGui::GetCurrentContext() != context) {
	    ImGui::SetCurrentContext(context);
    }
    // Shutdown ImGui
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
	
    //SDL_DestroyTexture(texture);
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }    
}

SDL_AppResult AlternateMainApplication::callback(SDL_Event* event) {
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

void AlternateMainApplication::draw() {

}

void AlternateMainApplication::drawMenu() {
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
}

SDL_AppResult AlternateMainApplication::initialize() {
    if (!window) {
        window = SDL_CreateWindow("Main Application", DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_RESIZABLE);
        if (!window) {
            return SDL_APP_FAILURE;
        }
    }
    if (!renderer) {
        renderer = SDL_CreateRenderer(window, nullptr);
        if (!renderer) {
            return SDL_APP_FAILURE;
        }
    }    
    // Setup Dear ImGui
    //IMGUI_CHECKVERSION();
    context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult AlternateMainApplication::loop() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);

    if (ImGui::GetCurrentContext() != context) {
	    ImGui::SetCurrentContext(context);
    }
          // Start ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    drawMenu();

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
	
    return SDL_APP_CONTINUE;	
}
