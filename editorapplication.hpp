#pragma once
#include "application.hpp"
#include <opencv2/opencv.hpp>

SDL_Texture* matToTexture(const cv::Mat& mat, SDL_Renderer* renderer);

class EditorApplication : public Application {
	public:
		//EditorApplication() : EditorApplication(nullptr, nullptr) { }
		//EditorApplication(SDL_Window* w, SDL_Renderer* r) : window(w), renderer(r) { }
		EditorApplication() : Application() { }
		EditorApplication(SDL_Window* w, SDL_Renderer* r) : Application(w, r) { }
		~EditorApplication();
		SDL_AppResult loop() override;
		SDL_AppResult callback(SDL_Event* event) override;
		void draw() override;
		void drawMenu() override;
		SDL_AppResult initialize() override;
	protected:
		cv::Mat image;
		SDL_Texture* texture;
		ImTextureID imguiTexture = 0;
};

