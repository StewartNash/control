#pragma once
#include "application.hpp"
#include <opencv2/opencv.hpp>

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

class EditorApplication : public Application {
	public:
		EditorApplication(SDL_Window* w, SDL_Renderer* r) : Application(w, r) { }
		//void loop();
		//void callback(SDL_Event* event);
		void draw();
		void drawMenu();
	protected:
		void initialize();
		cv::Mat image;
		SDL_Texture* texture;
};

