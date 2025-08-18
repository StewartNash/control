#include "editorapplication.hpp"

// Convert OpenCV Mat to SDL_Texture
SDL_Texture* matToTexture(const cv::Mat& mat, SDL_Renderer* renderer) {
	// Convert to RGBA if needed
	cv::Mat converted;
	if (mat.channels() == 3) {
		cv::cvtColor(mat, converted, cv::COLOR_BGR2RGBA);
	} else if (mat.channels() == 1) {
		cv::cvtColor(mat, converted, cv::COLOR_GRAY2RGBA);
	} else {
		converted = mat; // Already RGBA
	}
	// Create SDL surface from raw pixels
	SDL_Surface* surface = SDL_CreateSurfaceFrom(
		converted.cols,
		converted.rows,
		SDL_PIXELFORMAT_RGBA32,
		(void*)converted.data,
		converted.step
	);
	if (!surface) {
		SDL_Log("Failed to create SDL surface: %s", SDL_GetError());
		return nullptr;
	}
	// Create texture from surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);
	
	return texture;
}

void EditorApplication::draw() {
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
	if (texture) {
		ImGui::Image((ImTextureID)texture, ImVec2((float)image.cols, (float)image.rows));
	}
	ImGui::End();
}

void EditorApplication::initialize() {
	image = cv::imread("/home/stewart/Repositories/control/data/img531.png");
	texture = matToTexture(image, renderer);
	
	// Setup Dear ImGui
	//IMGUI_CHECKVERSION();
	context = ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
}
