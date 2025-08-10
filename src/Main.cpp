#include "Button.h"
//#include "ImageCrop.h"
//#include "Random.h"
#define TINYFILEDIALOGS_IMPLEMENTATION
#include "FileDialog/tinyfiledialogs.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>
#include <typeinfo>
#include <vector>
#include <raylib.h>

// Initial window size
const int screenHeight{ 720 };
const int screenWidth{ 1280 };

enum class Scene {
	LOADING_SCENE,
	MENU_SCENE,						// Display menu (Play, Exit)
	BEGIN_PLAY_SCENE,				// Begin playing the puzzle (all animation included)
	CHOOSE_IMAGE_SCENE,				// Show scene on choosing image (built-in, custom)
	CROP_SLICE_IMAGE_SCENE,			// Square, Portrait, Landscape (3x3, 4x4, 5x5, ... 3x4, 3x5, 3x6, ... etc)
	EXIT_SCENE
};

Image imageManipulate(Image* myImage)
{
	int Width = myImage->width;
	int Height = myImage->height;
	// target size
	float maxWidth = GetScreenWidth(); // 1.1f;
	float maxHeight = GetScreenHeight(); // 1.1f;
	// Compute scale ratio while preserving aspect ratio
	float scaleX = maxWidth / Width;
	float scaleY = maxHeight / Height;
	float scale = std::min(scaleX, scaleY);
	// New dimensions
	auto newWidth = static_cast<int>(Width * scale);
	auto newHeight = static_cast<int>(Height * scale);

	ImageResize(myImage, newWidth, newHeight);

	return *myImage;
}

Image bgImageOverlayManipulate(Image* myImage)
{
	int Width = myImage->width;
	int Height = myImage->height;
	// target size
	float maxWidth = GetScreenWidth() / 1.3f;
	float maxHeight = GetScreenHeight() / 1.3f;
	// Compute scale ratio while preserving aspect ratio
	float scaleX = maxWidth / Width;
	float scaleY = maxHeight / Height;
	float scale = std::min(scaleX, scaleY);
	// New dimensions
	auto newWidth = static_cast<int>(Width * scale);
	auto newHeight = static_cast<int>(Height * scale);

	ImageResize(myImage, newWidth, newHeight);

	return *myImage;
}

int main()
{
	InitWindow(screenWidth, screenHeight, "PuzzleDePizzle");
	SetConfigFlags(FLAG_VSYNC_HINT);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowMinSize(screenWidth, screenHeight);
	MaximizeWindow();
	SetTargetFPS(60);

	// For updating the screen dimension
	int currentWindowWidth{};
	int currentWindowHeight{};

	// For scenes
	Scene currentScene{ Scene::MENU_SCENE };

	// Other resources for the window
	Image myBgImage{ LoadImage("Resources/Images/bg.png") };
	Texture2D myBgTexture{ LoadTextureFromImage(imageManipulate(&myBgImage)) };
	Image myBgImageOverlay{ LoadImage("Resources/Images/bgOverlay.png") };
	Texture2D myBgTextureOverlay{ LoadTextureFromImage(bgImageOverlayManipulate(&myBgImageOverlay)) };

	bool menu{ true };

	// Buttons
	Button startButton{ LoadImage("Resources/Images/play_button_up.png"), LoadImage("Resources/Images/play_button_down.png"), 0.99f };
	Button exitButton{ LoadImage("Resources/Images/exit_button_up.png"), LoadImage("Resources/Images/exit_button_down.png"), 0.99f };
	Button backButton{ LoadImage("Resources/Images/back_button_up.png"), LoadImage("Resources/Images/back_button_down.png"), 0.99f };

	// THE IMAGE PUZZLE ITSELF AND ITS VARIABLES
	const char* filter[] = { "*.png", "*.jpg", "*.bmp" };
	Image myPuzzleImage{ nullptr };
	Texture2D myPuzzleTexture{ 0 };

	while (!WindowShouldClose())
	{
		// MouseClickLocation
		Vector2 clickLocation = GetMousePosition();
		bool clickHolding = true;
		
		// LOCKING/UNLOCKING RESIZE WINDOW
		if (IsKeyPressed(KEY_R))
		{
			if (IsWindowState(FLAG_WINDOW_RESIZABLE)) {
				ClearWindowState(FLAG_WINDOW_RESIZABLE);
			}
			else SetWindowState(FLAG_WINDOW_RESIZABLE);
		}

		// Resize the texture and maintain quality if the screen dimension is updated
		if (currentWindowWidth != GetScreenWidth() || currentWindowHeight != GetScreenHeight())
		{
			if(myPuzzleImage.data != nullptr)		// If resizing window happens before import or when no image yet
			{
				if (myPuzzleTexture.id != 0) {
					UnloadTexture(myPuzzleTexture);
				}

				Image temp = ImageCopy(myPuzzleImage);
				myPuzzleTexture = LoadTextureFromImage(imageManipulate(&temp));
				UnloadImage(temp);
			}

			//	For background image
			if (myBgTexture.id != 0) {
				UnloadTexture(myBgTexture);
			}
			Image bgTemp = ImageCopy(myBgImage);
			myBgTexture = LoadTextureFromImage(imageManipulate(&bgTemp));
			UnloadImage(bgTemp);
			Image bgOverlayTemp = ImageCopy(myBgImageOverlay);
			myBgTextureOverlay = LoadTextureFromImage(bgImageOverlayManipulate(&bgOverlayTemp));
			UnloadImage(bgOverlayTemp);

			startButton.scaled(myBgTexture.width, myBgTexture.height);
			exitButton.scaled(myBgTexture.width, myBgTexture.height);
			backButton.scaled(myBgTexture.width, myBgTexture.height);
			
			currentWindowWidth = GetScreenWidth();
			currentWindowHeight = GetScreenHeight();
		}
		// Testing import
		if (IsKeyPressed(KEY_I)) {
			const char* filepath = tinyfd_openFileDialog(
				"Select an Image",
				"",
				3, filter,
				"Image files",
				0
			);

			if (filepath) {
				if (myPuzzleTexture.id != 0) {
					UnloadTexture(myPuzzleTexture);
				}

				myPuzzleImage = LoadImage(filepath); // Load image
				myPuzzleTexture = LoadTextureFromImage(imageManipulate(&myPuzzleImage));
			}
		}

		// Clicking functionality
		if (startButton.isPressed() && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			currentScene = Scene::CHOOSE_IMAGE_SCENE;
		}
		if (backButton.isPressed() && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && currentScene == Scene::CHOOSE_IMAGE_SCENE) {
			currentScene = Scene::MENU_SCENE;
		}
		
		// Choose Image puzzle
		
		// DISPLAY EVERYTHING HERE NOW
		BeginDrawing();
		ClearBackground(BLACK);

		DrawTexture(myBgTexture, (currentWindowWidth - myBgTexture.width) / 2, (currentWindowHeight - myBgTexture.height) / 2, RAYWHITE);

		if (myPuzzleTexture.id != 0) {
			DrawCircle(50, 50, 50, ORANGE);
			// Draw texture in the middle of the screen
			DrawTexture(myPuzzleTexture, (currentWindowWidth - myPuzzleTexture.width) / 2, (currentWindowHeight - myPuzzleTexture.height) / 2, WHITE);
		}

		DrawText("Press [I] to import an image", 10, 10, 20, DARKGRAY);

		switch (currentScene)
		{
			case Scene::MENU_SCENE: {
				startButton.draw({ (currentWindowWidth - startButton.getButtonWidth()) / 2, (currentWindowHeight - startButton.getButtonHeight()) / 1.8f }, clickLocation);
				exitButton.draw({ (currentWindowWidth - exitButton.getButtonWidth()) / 2, (currentWindowHeight - exitButton.getButtonHeight()) - startButton.getButtonHeight() }, clickLocation);
			} break;

			case Scene::CHOOSE_IMAGE_SCENE: {
				DrawTexture(myBgTextureOverlay, (currentWindowWidth - myBgTextureOverlay.width) / 2, (currentWindowHeight - myBgTextureOverlay.height) / 2, WHITE);
				backButton.draw({ (currentWindowWidth - startButton.getButtonWidth()) / 6, (currentWindowHeight - startButton.getButtonHeight()) / 7.0f }, clickLocation);
			} break;
		}

		EndDrawing();
	}

	UnloadImage(myBgImage);
	UnloadImage(myBgImageOverlay);
	UnloadImage(myPuzzleImage);
	UnloadTexture(myPuzzleTexture);
	UnloadTexture(myBgTexture);

	CloseWindow();

	return 0;
}