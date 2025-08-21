#include "editorapplication.hpp"

SDL_Texture* matToTexture(const cv::Mat& mat, SDL_Renderer* renderer) {
    if (mat.empty()) {
        SDL_Log("matToTexture: input Mat is empty!");
        return nullptr;
    }

    cv::Mat converted;

    // Convert to RGBA
    if (mat.channels() == 3) {
        cv::cvtColor(mat, converted, cv::COLOR_BGR2RGBA);
    } else if (mat.channels() == 1) {
        cv::cvtColor(mat, converted, cv::COLOR_GRAY2RGBA);
    } else if (mat.channels() == 4) {
        converted = mat.clone();
    } else {
        SDL_Log("matToTexture: Unsupported number of channels: %d", mat.channels());
        return nullptr;
    }

    // Create a STREAMING texture so we can update it
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        converted.cols,
        converted.rows
    );

    if (!texture) {
        SDL_Log("matToTexture: SDL_CreateTexture failed: %s", SDL_GetError());
        return nullptr;
    }

    // Update the texture with pixel data
    if (SDL_UpdateTexture(texture, nullptr, converted.data, converted.step) != 0) {
        SDL_Log("matToTexture: SDL_UpdateTexture failed: %s", SDL_GetError());
        SDL_DestroyTexture(texture);
        return nullptr;
    }

    SDL_Log("matToTexture: texture created successfully (%dx%d)", converted.cols, converted.rows);
    return texture;
}


void EditorApplication::draw() {
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
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

	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, texture, nullptr, nullptr);

	ImGui::Render();	
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

	SDL_RenderPresent(renderer);
}

void EditorApplication::drawMenu() {
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
}

void EditorApplication::initialize() {
    // 1. ImGui context and backend
    context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // 2. Load image
    image = cv::imread("/home/stewart/Repositories/control/data/img531.png", cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        SDL_Log("EditorApplication::initialize: failed to load image");
        return;
    }

    SDL_Log("EditorApplication::initialize: image size %d x %d", image.cols, image.rows);

    // 3. Convert to SDL_Texture
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    texture = matToTexture(image, renderer);
    if (!texture) {
        SDL_Log("EditorApplication::initialize: matToTexture failed");
        return;
    }

    SDL_Log("EditorApplication::initialize: texture created successfully");

    // 4. Assign to ImGui
    imguiTexture = (ImTextureID)(intptr_t)texture;
}

////////////////////////////////
// AlternateEditorApplication //
////////////////////////////////

AlternateEditorApplication::~AlternateEditorApplication() {
    SDL_DestroyTexture(texture);
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }    
}

SDL_AppResult AlternateEditorApplication::callback(SDL_Event* event) {
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

void AlternateEditorApplication::draw() {
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

void AlternateEditorApplication::drawMenu() {
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

SDL_WindowID AlternateEditorApplication::getWindowID() {
    return SDL_GetWindowID(window);
}

SDL_AppResult AlternateEditorApplication::initialize() {
    if (!window) {
        window = SDL_CreateWindow("Image Editor", DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_RESIZABLE);
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
    image = cv::imread("../data/img531.png");
    if (image.empty()) {
        SDL_Log("Failed to load image with OpenCV");
        return SDL_APP_FAILURE;
    }
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STATIC,
        image.cols,
        image.rows
    );
    if (!texture) {
        return SDL_APP_FAILURE;
    }
    SDL_UpdateTexture(texture, nullptr, image.data, image.step);

    return SDL_APP_CONTINUE;
}

SDL_AppResult AlternateEditorApplication::loop() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if (texture) {
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    }
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

