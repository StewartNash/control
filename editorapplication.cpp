#include "editorapplication.hpp"

void EditorApplication::draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
	
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
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
}
