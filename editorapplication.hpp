#pragma once
#include "application.hpp"
#include <opencv2/opencv.hpp>

SDL_Texture* matToTexture(const cv::Mat& mat, SDL_Renderer* renderer);

class EditorApplication : public Application {
	public:
		//EditorApplication();
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

class AlternateEditorApplication {
	public:
		AlternateEditorApplication() : AlternateEditorApplication(nullptr, nullptr) { }
		AlternateEditorApplication(SDL_Window* w, SDL_Renderer* r) : window(w), renderer(r) { }
		~AlternateEditorApplication();
		SDL_AppResult loop();
		SDL_AppResult callback(SDL_Event* event);
		void draw();
		void drawMenu();
		SDL_AppResult initialize();
		SDL_WindowID getWindowID();
	protected:
		cv::Mat image;
		SDL_Texture* texture;
		ImTextureID imguiTexture = 0;
		SDL_Window* window;
		SDL_Renderer* renderer;
		const int DEFAULT_WIDTH = 960;
		const int DEFAULT_HEIGHT = 720;
};

