#include "application.hpp"

Application::~Application() {
	if (ImGui::GetCurrentContext() != context) {
		ImGui::SetCurrentContext(context);
	}
	// Shutdown ImGui
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void Application::callback(SDL_Event* event) {
	if (!event) {
		return;
	}
	if (!context) {
		return;
	}
	if (!window) {
		return;
	}
	if (ImGui::GetCurrentContext() != context) {
		ImGui::SetCurrentContext(context);
	}
	IM_ASSERT(ImGui::GetCurrentContext() == context);
	// Pass events to ImGui
	ImGui_ImplSDL3_ProcessEvent(event);
}

void Application::draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);

	if (ImGui::GetCurrentContext() != context) {
		ImGui::SetCurrentContext(context);
	}
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

void Application::drawMenu() {
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

void Application::initialize() {
	// Setup Dear ImGui
	//IMGUI_CHECKVERSION();
	context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);
	
	ImGui::StyleColorsDark();
	
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
}

void Application::loop() {
	draw();
}

//////////////////////////
// AlternateApplication //
//////////////////////////

AlternateApplication::~AlternateApplication() {
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

SDL_AppResult AlternateApplication::callback(SDL_Event* event) {
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

void AlternateApplication::draw() {
	/*
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
	if (ImGui::GetCurrentContext() != context) {
		ImGui::SetCurrentContext(context);
	}
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	drawMenu();
	//
	//for (const auto& blk : state->blocks) {
	//	SDL_FRect rect = {
	//		static_cast<float>(blk.x),
	//		static_cast<float>(blk.y),
	//		static_cast<float>(blk.width),
	//		static_cast<float>(blk.height)};
	//	SDL_RenderFillRect(state->renderer, &rect);
	//}
	//
	// Render ImGui

	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, texture, nullptr, nullptr);

	ImGui::Render();	
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

	SDL_RenderPresent(renderer);
	*/
}

void AlternateApplication::drawMenu() {
    /*
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
    ImGui::Text("Image cols=%d rows=%d", image.cols, image.rows);
          if (imguiTexture) {
              ImVec2 avail = ImGui::GetContentRegionAvail();
              // optionally scale to fit: keep aspect ratio
              float aspect = (float)image.cols / (float)image.rows;
              float w = std::min((float)image.cols, avail.x);
              float h = w / aspect;
              if (h > avail.y) { h = avail.y; w = h * aspect; }
              ImGui::Image(imguiTexture, ImVec2(w, h));
          } else {
              ImGui::Text("No texture!");
          }
    ImGui::End();
    */
}

SDL_WindowID AlternateApplication::getWindowID() {
    return SDL_GetWindowID(window);
}

SDL_AppResult AlternateApplication::initialize() {
    if (!window) {
        window = SDL_CreateWindow("Control Application", DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_RESIZABLE);
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

    return SDL_APP_CONTINUE;
}

SDL_AppResult AlternateApplication::loop() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    /*
    if (texture) {
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    }
    */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

