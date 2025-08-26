//#include "assets.h"
#include "Button.h"
#include "ImageCrop.h"
//#include "image_manipulate.h"
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
};

enum ImageType {
	IMAGE_AS_BG,
	IMAGE_AS_BG_OVERLAY,
	IMAGE_AS_ICON,
	IMAGE_AS_PUZZLE
};

Image imageManipulate(Image* myImage, ImageType imageType)
{
	int Width = myImage->width;
	int Height = myImage->height;

	// target size
	float maxWidth{};
	float maxHeight{};
	// Compute scale ratio while preserving aspect ratio
	float scaleX{};
	float scaleY{};
	float scale{};
	// New dimensions
	int newWidth{};
	int newHeight{};

	if (imageType == IMAGE_AS_ICON) {
		maxWidth = GetScreenWidth() / 5.0f;
		maxHeight = GetScreenHeight() / 5.0f;
		scaleX = maxWidth / Width;
		scaleY = maxHeight / Height;
		scale = std::min(scaleX, scaleY);

		newWidth = static_cast<int>(Width * scale);
		newHeight = static_cast<int>(Height * scale);
	}
	else if (imageType == IMAGE_AS_BG_OVERLAY) {
		maxWidth = GetScreenWidth() / 1.3f;
		maxHeight = GetScreenHeight() / 1.3f;
		scaleX = maxWidth / Width;
		scaleY = maxHeight / Height;
		scale = std::min(scaleX, scaleY);

		newWidth = static_cast<int>(Width * scale);
		newHeight = static_cast<int>(Height * scale);
	}
	else if (imageType == IMAGE_AS_PUZZLE) {
		maxWidth = GetScreenWidth() / 1.5f;
		maxHeight = GetScreenHeight() / 1.5f;
		scaleX = maxWidth / Width;
		scaleY = maxHeight / Height;
		scale = std::min(scaleX, scaleY);

		newWidth = static_cast<int>(Width * scale);
		newHeight = static_cast<int>(Height * scale);
	}
	else {
		maxWidth = GetScreenWidth();
		maxHeight = GetScreenHeight();
		scaleX = maxWidth / Width;
		scaleY = maxHeight / Height;
		scale = std::min(scaleX, scaleY);

		newWidth = static_cast<int>(Width * scale);
		newHeight = static_cast<int>(Height * scale);
	}

	ImageResize(myImage, newWidth, newHeight);

	return *myImage;
}

void theImagePuzzle(Image& myPuzzleImage, const Image& myImageChoosen, Texture& myPuzzleTexture)
{
	myPuzzleImage = ImageCopy(myImageChoosen);
	myPuzzleTexture = LoadTextureFromImage(imageManipulate(&myPuzzleImage, IMAGE_AS_PUZZLE));
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
	Texture2D myBgTexture{ LoadTextureFromImage(myBgImage) };
	Image myBgImageOverlay{ LoadImage("Resources/Images/bgOverlay.png") };
	Texture2D myBgTextureOverlay{ LoadTextureFromImage(myBgImageOverlay) };

	// Buttons
	Button startButton{ LoadImage("Resources/Images/play_button_up.png"), LoadImage("Resources/Images/play_button_down.png"), myBgTexture.width, myBgTexture.height, MEDIUM };
	Button exitButton{ LoadImage("Resources/Images/exit_button_up.png"), LoadImage("Resources/Images/exit_button_down.png"), myBgTexture.width, myBgTexture.height, MEDIUM };
	Button backButton{ LoadImage("Resources/Images/back_button_up.png"), LoadImage("Resources/Images/back_button_down.png"), myBgTexture.width, myBgTexture.height, MEDIUM };

	// Game resources
	// Images
	Image puzzleImage1{ LoadImage("Resources/Images/bertface.png") };
	Texture2D puzzleImage1Texture{ LoadTextureFromImage(puzzleImage1) };
	Image puzzleImage2{ LoadImage("Resources/Images/jerry.jpg") };
	Texture2D puzzleImage2Texture{ LoadTextureFromImage(puzzleImage2) };
	Image puzzleImage3{ LoadImage("Resources/Images/mayon.jpg") };
	Texture2D puzzleImage3Texture{ LoadTextureFromImage(puzzleImage3) };

	// Font
	float fontSizeLarge{ 70.0f };
	float fontSizeSmall{ 50.0f };
	float fontSpacing{ 2.0f };
	Font myFontLarge{ LoadFontEx("Resources/Font/The Bomb Sound.ttf", fontSizeLarge, nullptr, 0) };
	Font myFontSmall{ LoadFontEx("Resources/Font/The Bomb Sound.ttf", fontSizeSmall, nullptr, 0) };


	// Texts
	Image txt_ChooseImage{ ImageTextEx(myFontLarge, "Choose Image", fontSizeLarge, fontSpacing, BLACK) };
	Texture2D txt_ChooseImage_texture{ LoadTextureFromImage(txt_ChooseImage) };

	Image puzImg1Txt{ ImageTextEx(myFontLarge, "Bert Face", fontSizeLarge, fontSpacing, BLACK) };
	Texture2D puzImg1Txt_texture{ LoadTextureFromImage(puzImg1Txt) };
	Image puzImg2Txt{ ImageTextEx(myFontLarge, "High Jerry", fontSizeLarge, fontSpacing, BLACK) };
	Texture2D puzImg2Txt_texture{ LoadTextureFromImage(puzImg2Txt) };
	Image puzImg3Txt{ ImageTextEx(myFontLarge, "Mayon", fontSizeLarge, fontSpacing, BLACK) };
	Texture2D puzImg3Txt_texture{ LoadTextureFromImage(puzImg3Txt) };
	

	// THE IMAGE PUZZLE ITSELF AND ITS VARIABLES
	const char* filter[] = { "*.png", "*.jpg", "*.bmp" };
	Image myPuzzleImage{ nullptr };
	Texture2D myPuzzleTexture{ 0 };


	bool hoovered1{ false };
	bool hoovered2{ false };
	bool hoovered3{ false };


	while (!WindowShouldClose())
	{
		// MouseClickLocation
		Vector2 clickLocation = GetMousePosition();
		
		// LOCKING/UNLOCKING RESIZE WINDOW
		if (IsKeyPressed(KEY_R))
		{
			if (IsWindowState(FLAG_WINDOW_RESIZABLE)) {
				ClearWindowState(FLAG_WINDOW_RESIZABLE);
			}
			else SetWindowState(FLAG_WINDOW_RESIZABLE);
		}


		// Clicking functionality
		if (startButton.isPressed() && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && currentScene == Scene::MENU_SCENE) {
			currentScene = Scene::CHOOSE_IMAGE_SCENE;
		}
		if (backButton.isPressed() && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && currentScene == Scene::CHOOSE_IMAGE_SCENE) {
			currentScene = Scene::MENU_SCENE;
		}
		if (currentScene == Scene::CHOOSE_IMAGE_SCENE) {
			if (hoovered1 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				theImagePuzzle(myPuzzleImage, puzzleImage1, myPuzzleTexture);
				currentScene = Scene::CROP_SLICE_IMAGE_SCENE;
			}
			if (hoovered2 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				theImagePuzzle(myPuzzleImage, puzzleImage2, myPuzzleTexture);
				currentScene = Scene::CROP_SLICE_IMAGE_SCENE;
			}
			if (hoovered3 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				theImagePuzzle(myPuzzleImage, puzzleImage3, myPuzzleTexture);
				currentScene = Scene::CROP_SLICE_IMAGE_SCENE;
			}
		}
		if (backButton.isPressed() && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && currentScene == Scene::CROP_SLICE_IMAGE_SCENE) {
			if (myPuzzleTexture.id != 0) {
				UnloadTexture(myPuzzleTexture);
			}
			currentScene = Scene::CHOOSE_IMAGE_SCENE;
		}

		// Resize the texture and maintain quality if the screen dimension is updated
		if (currentWindowWidth != GetScreenWidth() || currentWindowHeight != GetScreenHeight())
		{
			if(myPuzzleImage.data != nullptr)
			{
				if (myPuzzleTexture.id != 0) {
					UnloadTexture(myPuzzleTexture);
				}

				Image temp = ImageCopy(myPuzzleImage);
				myPuzzleTexture = LoadTextureFromImage(imageManipulate(&temp, IMAGE_AS_PUZZLE));
				UnloadImage(temp);
			}

			//	For background image
			if (myBgTexture.id != 0) {
				UnloadTexture(myBgTexture);
			}
			Image bgTemp = ImageCopy(myBgImage);
			myBgTexture = LoadTextureFromImage(imageManipulate(&bgTemp, IMAGE_AS_BG));
			UnloadImage(bgTemp);
			Image bgOverlayTemp = ImageCopy(myBgImageOverlay);
			myBgTextureOverlay = LoadTextureFromImage(imageManipulate(&bgOverlayTemp, IMAGE_AS_BG_OVERLAY));
			UnloadImage(bgOverlayTemp);

			// For Icons
			if (puzzleImage1Texture.id != 0) {
				UnloadTexture(puzzleImage1Texture);
				UnloadTexture(puzzleImage2Texture);
				UnloadTexture(puzzleImage3Texture);
			}
			Image puzIconTemp = ImageCopy(puzzleImage1);
			puzzleImage1Texture = LoadTextureFromImage(imageManipulate(&puzIconTemp, IMAGE_AS_ICON));
			UnloadImage(puzIconTemp);
			puzIconTemp = ImageCopy(puzzleImage2);
			puzzleImage2Texture = LoadTextureFromImage(imageManipulate(&puzIconTemp, IMAGE_AS_ICON));
			UnloadImage(puzIconTemp);
			puzIconTemp = ImageCopy(puzzleImage3);
			puzzleImage3Texture = LoadTextureFromImage(imageManipulate(&puzIconTemp, IMAGE_AS_ICON));
			UnloadImage(puzIconTemp);

			// Buttons
			startButton.scaled(myBgTexture.width, myBgTexture.height, LARGE);
			exitButton.scaled(myBgTexture.width, myBgTexture.height, LARGE);
			backButton.scaled(myBgTexture.width, myBgTexture.height, MEDIUM);

			// Texts
			if (txt_ChooseImage_texture.id != 0) {
				UnloadTexture(txt_ChooseImage_texture);
				UnloadTexture(puzImg1Txt_texture);
				UnloadTexture(puzImg2Txt_texture);
				UnloadTexture(puzImg3Txt_texture);
			}
			Image tempText = ImageCopy(txt_ChooseImage);
			txt_ChooseImage_texture = LoadTextureFromImage(imageManipulate(&tempText, IMAGE_AS_ICON));
			UnloadImage(tempText);
			tempText = ImageCopy(puzImg1Txt);
			puzImg1Txt_texture = LoadTextureFromImage(imageManipulate(&tempText, IMAGE_AS_ICON));
			UnloadImage(tempText);
			tempText = ImageCopy(puzImg2Txt);
			puzImg2Txt_texture = LoadTextureFromImage(imageManipulate(&tempText, IMAGE_AS_ICON));
			UnloadImage(tempText);
			tempText = ImageCopy(puzImg3Txt);
			puzImg3Txt_texture = LoadTextureFromImage(imageManipulate(&tempText, IMAGE_AS_ICON));
			UnloadImage(tempText);
			
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
				myPuzzleTexture = LoadTextureFromImage(imageManipulate(&myPuzzleImage, IMAGE_AS_PUZZLE));
			}
		}
		
		// Built-In Image Puzzle Selection
		Rectangle puz1ImgLoc = { ((currentWindowWidth - myBgTextureOverlay.width) / 2.0f), (myBgTextureOverlay.height * 0.1f) + ((currentWindowHeight - myBgTextureOverlay.height) / 2.0f), myBgTextureOverlay.width, puzzleImage1Texture.height };
		Rectangle puz2ImgLoc = { ((currentWindowWidth - myBgTextureOverlay.width) / 2.0f), (myBgTextureOverlay.height * 0.1f) + (puzzleImage1Texture.height * 1.05f) + ((currentWindowHeight - myBgTextureOverlay.height) / 2.0f), myBgTextureOverlay.width, puzzleImage2Texture.height };
		Rectangle puz3ImgLoc = { ((currentWindowWidth - myBgTextureOverlay.width) / 2.0f), (myBgTextureOverlay.height * 0.1f) + (puzzleImage1Texture.height * 1.05f) + (puzzleImage2Texture.height * 1.05f) + ((currentWindowHeight - myBgTextureOverlay.height) / 2.0f), myBgTextureOverlay.width, puzzleImage3Texture.height };
		
		// DISPLAY EVERYTHING HERE NOW
		BeginDrawing();
		ClearBackground(BLACK);

		DrawTexture(myBgTexture, (currentWindowWidth - myBgTexture.width) / 2, (currentWindowHeight - myBgTexture.height) / 2, RAYWHITE);

		DrawText("Press [I] to import an image", 10, 10, 20, DARKGRAY);

		switch (currentScene)
		{
			case Scene::MENU_SCENE: 
			{
				startButton.draw({ (currentWindowWidth / 2.0f) - (startButton.getButtonWidth() / 2.0f), (currentWindowHeight / 1.9f) }, clickLocation);
				exitButton.draw({ (currentWindowWidth / 2.0f) - (exitButton.getButtonWidth() / 2.0f), (currentWindowHeight / 1.9f) + (startButton.getButtonHeight() * 1.1f) }, clickLocation);
			} break;

			case Scene::CHOOSE_IMAGE_SCENE: 
			{
				DrawTexture(myBgTextureOverlay, (currentWindowWidth - myBgTextureOverlay.width) / 2, (currentWindowHeight - myBgTextureOverlay.height) / 2, WHITE);
				backButton.draw({ (currentWindowWidth - myBgTexture.width) / 2 + 10.0f, (currentWindowHeight - myBgTexture.height) + 10.0f }, clickLocation);

				DrawTexture(txt_ChooseImage_texture, (myBgTextureOverlay.width / 2) - (txt_ChooseImage_texture.width / 2) + ((currentWindowWidth - myBgTextureOverlay.width) / 2), ((currentWindowHeight - myBgTextureOverlay.height) / 2) - (txt_ChooseImage_texture.height / 2), WHITE);

				if (clickLocation.x >= puz1ImgLoc.x && clickLocation.x < (puz1ImgLoc.x + puz1ImgLoc.width) && clickLocation.y >= puz1ImgLoc.y && clickLocation.y < (puz1ImgLoc.y + puzzleImage1Texture.height)) {
					DrawRectangle((currentWindowWidth - myBgTextureOverlay.width) / 2, puz1ImgLoc.y, myBgTextureOverlay.width, puzzleImage1Texture.height, YELLOW);
					hoovered1 = true;
				}
				else if (clickLocation.x >= puz2ImgLoc.x && clickLocation.x < (puz2ImgLoc.x + puz2ImgLoc.width) && clickLocation.y >= puz2ImgLoc.y && clickLocation.y < (puz2ImgLoc.y + puzzleImage2Texture.height)) {
					DrawRectangle((currentWindowWidth - myBgTextureOverlay.width) / 2, puz2ImgLoc.y, myBgTextureOverlay.width, puzzleImage2Texture.height, YELLOW);
					hoovered2 = true;
				}
				else if (clickLocation.x >= puz3ImgLoc.x && clickLocation.x < (puz3ImgLoc.x + puz3ImgLoc.width) && clickLocation.y >= puz3ImgLoc.y && clickLocation.y < (puz3ImgLoc.y + puzzleImage3Texture.height)) {
					DrawRectangle((currentWindowWidth - myBgTextureOverlay.width) / 2, puz3ImgLoc.y, myBgTextureOverlay.width, puzzleImage3Texture.height, YELLOW);
					hoovered3 = true;
				}
				else {
					hoovered1 = false;
					hoovered2 = false;
					hoovered3 = false;
				}
				
				DrawTexture(puzzleImage1Texture, (myBgTextureOverlay.width * 0.1f) + ((currentWindowWidth - myBgTextureOverlay.width) / 2.0f), (myBgTextureOverlay.height * 0.1f) + ((currentWindowHeight - myBgTextureOverlay.height) / 2.0f), WHITE);
				DrawTexture(puzzleImage2Texture, (myBgTextureOverlay.width * 0.1f) + ((currentWindowWidth - myBgTextureOverlay.width) / 2.0f), (myBgTextureOverlay.height * 0.1f) + (puzzleImage1Texture.height * 1.05f) + ((currentWindowHeight - myBgTextureOverlay.height) / 2.0f), WHITE);
				DrawTexture(puzzleImage3Texture, (myBgTextureOverlay.width * 0.1f) + ((currentWindowWidth - myBgTextureOverlay.width) / 2.0f), (myBgTextureOverlay.height * 0.1f) + (puzzleImage1Texture.height * 1.05f) + (puzzleImage2Texture.height * 1.05f) + ((currentWindowHeight - myBgTextureOverlay.height) / 2.0f), WHITE);
				
				DrawTexture(puzImg1Txt_texture, (currentWindowWidth / 2.0f), (myBgTextureOverlay.height * 0.1f) + ((currentWindowHeight - myBgTextureOverlay.height) / 2.0f), BLACK);
				DrawTexture(puzImg2Txt_texture, (currentWindowWidth / 2.0f), (myBgTextureOverlay.height * 0.1f) + (puzzleImage1Texture.height * 1.05f) + ((currentWindowHeight - myBgTextureOverlay.height) / 2.0f), BLACK);
				DrawTexture(puzImg3Txt_texture, (currentWindowWidth / 2.0f), (myBgTextureOverlay.height * 0.1f) + (puzzleImage1Texture.height * 1.05f) + (puzzleImage2Texture.height * 1.05f) + ((currentWindowHeight - myBgTextureOverlay.height) / 2.0f), BLACK);

			} break;

			case Scene::CROP_SLICE_IMAGE_SCENE: 
			{
				backButton.draw({ (currentWindowWidth - myBgTexture.width) / 2 + 10.0f, (currentWindowHeight - myBgTexture.height) + 20.0f }, clickLocation);
				if (myPuzzleTexture.id != 0) {
					// Draw texture in the middle of the screen
					DrawTexture(myPuzzleTexture, (currentWindowWidth - myPuzzleTexture.width) / 2, (currentWindowHeight - myPuzzleTexture.height) / 2, WHITE);
				}
				hoovered1 = false;
				hoovered2 = false;
				hoovered3 = false;
			} break;
		}
		std::cout << static_cast<int>(currentScene) << '\n';

		EndDrawing();
	}

	UnloadImage(myBgImage);
	UnloadTexture(myBgTexture);
	UnloadImage(myBgImageOverlay);
	UnloadTexture(myBgTextureOverlay);
	UnloadImage(puzzleImage1);
	UnloadTexture(puzzleImage1Texture);
	UnloadImage(puzzleImage2);
	UnloadTexture(puzzleImage2Texture);
	UnloadImage(puzzleImage3);
	UnloadTexture(puzzleImage3Texture);
	UnloadImage(myPuzzleImage);
	UnloadTexture(myPuzzleTexture);
	
	UnloadImage(txt_ChooseImage);
	UnloadTexture(txt_ChooseImage_texture);

	UnloadImage(puzImg1Txt);
	UnloadTexture(puzImg1Txt_texture);
	UnloadImage(puzImg2Txt);
	UnloadTexture(puzImg2Txt_texture);
	UnloadImage(puzImg3Txt);
	UnloadTexture(puzImg3Txt_texture);

	UnloadFont(myFontLarge);
	UnloadFont(myFontSmall);

	CloseWindow();

	return 0;
}