#pragma once
#include "application.hpp"
#include <opencv2/opencv.hpp>

SDL_Texture* matToTexture(const cv::Mat& mat, SDL_Renderer* renderer);

class EditorApplication : public Application {
	public:
		EditorApplication(SDL_Window* w, SDL_Renderer* r) : Application(w, r) { }
		//void loop();
		//void callback(SDL_Event* event);
		void draw() override;
		void drawMenu() override;
		void initialize() override;
	protected:
		cv::Mat image;
		SDL_Texture* texture;
		ImTextureID imguiTexture = 0;
};

