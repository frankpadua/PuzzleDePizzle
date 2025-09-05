#include "assets.h"
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
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "PuzzleDePizzle");
	
	//SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowMinSize(screenWidth, screenHeight);
	MaximizeWindow();
	SetTargetFPS(60);

	LoadAssets();
	gA::LoadClassedAssets();

	while (!WindowShouldClose())
	{
		// MouseClickLocation
		gc.clickLocation = GetMousePosition();
		
		// LOCKING/UNLOCKING RESIZE WINDOW
		if (IsKeyPressed(KEY_R))
		{
			if (IsWindowState(FLAG_WINDOW_RESIZABLE)) {
				ClearWindowState(FLAG_WINDOW_RESIZABLE);
			}
			else SetWindowState(FLAG_WINDOW_RESIZABLE);
		}

		// Clicking functionality
		// SCENE 1
		if (gc.currentScene == Scene::MENU_SCENE) {
			// Scene 1-2 switch
			if (gA::startButton.isPressed()) {
				gc.currentScene = Scene::CHOOSE_IMAGE_SCENE;
			}
		}
		// SCENE 2
		if (gc.currentScene == Scene::CHOOSE_IMAGE_SCENE) {
			// Scene 2-1 switch
			if (gA::backButton.isPressed()) {
				gc.currentScene = Scene::MENU_SCENE;
			}
			// Scene 2-3 switch
			if (gc.puz1hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				theImagePuzzle(ga.myPuzzleImage, ga.puzzleImage1, ga.myPuzzleTexture);
				gc.currentScene = Scene::CROP_SLICE_IMAGE_SCENE;
			}
			if (gc.puz2hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				theImagePuzzle(ga.myPuzzleImage, ga.puzzleImage2, ga.myPuzzleTexture);
				gc.currentScene = Scene::CROP_SLICE_IMAGE_SCENE;
			}
			if (gc.puz3hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				theImagePuzzle(ga.myPuzzleImage, ga.puzzleImage3, ga.myPuzzleTexture);
				gc.currentScene = Scene::CROP_SLICE_IMAGE_SCENE;
			}
		}
		// SCENE 3
		if (gc.currentScene == Scene::CROP_SLICE_IMAGE_SCENE) {
			// Scene 3-2 switch
			if (gA::backButton.isPressed()) {
				if (ga.myPuzzleTexture.id != 0) {
					UnloadTexture(ga.myPuzzleTexture);
				}
				gc.currentScene = Scene::CHOOSE_IMAGE_SCENE;
			}
		}

		// Resize the texture and maintain quality if the screen dimension is updated
		if (gc.currentWindowWidth != GetScreenWidth() || gc.currentWindowHeight != GetScreenHeight())
		{
			if(ga.myPuzzleImage.data != nullptr)
			{
				if (ga.myPuzzleTexture.id != 0) {
					UnloadTexture(ga.myPuzzleTexture);
				}

				Image temp = ImageCopy(ga.myPuzzleImage);
				ga.myPuzzleTexture = LoadTextureFromImage(imageManipulate(&temp, IMAGE_AS_PUZZLE));
				UnloadImage(temp);
			}

			//	For background image
			if (ga.myBgTexture.id != 0) {
				UnloadTexture(ga.myBgTexture);
			}
			Image bgTemp = ImageCopy(ga.myBgImage);
			ga.myBgTexture = LoadTextureFromImage(imageManipulate(&bgTemp, IMAGE_AS_BG));
			UnloadImage(bgTemp);
			Image bgOverlayTemp = ImageCopy(ga.myBgImageOverlay);
			ga.myBgTextureOverlay = LoadTextureFromImage(imageManipulate(&bgOverlayTemp, IMAGE_AS_BG_OVERLAY));
			UnloadImage(bgOverlayTemp);

			// For Icons
			if (ga.puzzleImage1Texture.id != 0) {
				UnloadTexture(ga.puzzleImage1Texture);
				UnloadTexture(ga.puzzleImage2Texture);
				UnloadTexture(ga.puzzleImage3Texture);
			}
			Image puzIconTemp = ImageCopy(ga.puzzleImage1);
			ga.puzzleImage1Texture = LoadTextureFromImage(imageManipulate(&puzIconTemp, IMAGE_AS_ICON));
			UnloadImage(puzIconTemp);
			puzIconTemp = ImageCopy(ga.puzzleImage2);
			ga.puzzleImage2Texture = LoadTextureFromImage(imageManipulate(&puzIconTemp, IMAGE_AS_ICON));
			UnloadImage(puzIconTemp);
			puzIconTemp = ImageCopy(ga.puzzleImage3);
			ga.puzzleImage3Texture = LoadTextureFromImage(imageManipulate(&puzIconTemp, IMAGE_AS_ICON));
			UnloadImage(puzIconTemp);

			// Buttons
			gA::startButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, LARGE);
			gA::exitButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, LARGE);
			gA::backButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, MEDIUM);

			// Texts
			if (ga.txt_ChooseImage_texture.id != 0) {
				UnloadTexture(ga.txt_ChooseImage_texture);
				UnloadTexture(ga.puzImg1Txt_texture);
				UnloadTexture(ga.puzImg2Txt_texture);
				UnloadTexture(ga.puzImg3Txt_texture);
			}
			Image tempText = ImageCopy(ga.txt_ChooseImage);
			ga.txt_ChooseImage_texture = LoadTextureFromImage(imageManipulate(&tempText, IMAGE_AS_ICON));
			UnloadImage(tempText);
			tempText = ImageCopy(ga.puzImg1Txt);
			ga.puzImg1Txt_texture = LoadTextureFromImage(imageManipulate(&tempText, IMAGE_AS_ICON));
			UnloadImage(tempText);
			tempText = ImageCopy(ga.puzImg2Txt);
			ga.puzImg2Txt_texture = LoadTextureFromImage(imageManipulate(&tempText, IMAGE_AS_ICON));
			UnloadImage(tempText);
			tempText = ImageCopy(ga.puzImg3Txt);
			ga.puzImg3Txt_texture = LoadTextureFromImage(imageManipulate(&tempText, IMAGE_AS_ICON));
			UnloadImage(tempText);
			
			gc.currentWindowWidth = GetScreenWidth();
			gc.currentWindowHeight = GetScreenHeight();
		}
		// Testing import
		if (IsKeyPressed(KEY_I)) {
			const char* filepath = tinyfd_openFileDialog(
				"Select an Image",
				"",
				3, FILTER,
				"Image files",
				0
			);

			if (filepath) {
				if (ga.myPuzzleTexture.id != 0) {
					UnloadTexture(ga.myPuzzleTexture);
				}

				ga.myPuzzleImage = LoadImage(filepath); // Load image
				ga.myPuzzleTexture = LoadTextureFromImage(imageManipulate(&ga.myPuzzleImage, IMAGE_AS_PUZZLE));
			}
		}
		
		// Built-In Image Puzzle Selection
		Rectangle puz1ImgLoc = { ((gc.currentWindowWidth - ga.myBgTextureOverlay.width) / 2.0f), (ga.myBgTextureOverlay.height * 0.1f) + ((gc.currentWindowHeight - ga.myBgTextureOverlay.height) / 2.0f), ga.myBgTextureOverlay.width, ga.puzzleImage1Texture.height };
		Rectangle puz2ImgLoc = { ((gc.currentWindowWidth - ga.myBgTextureOverlay.width) / 2.0f), (ga.myBgTextureOverlay.height * 0.1f) + (ga.puzzleImage1Texture.height * 1.05f) + ((gc.currentWindowHeight - ga.myBgTextureOverlay.height) / 2.0f), ga.myBgTextureOverlay.width, ga.puzzleImage2Texture.height };
		Rectangle puz3ImgLoc = { ((gc.currentWindowWidth - ga.myBgTextureOverlay.width) / 2.0f), (ga.myBgTextureOverlay.height * 0.1f) + (ga.puzzleImage1Texture.height * 1.05f) + (ga.puzzleImage2Texture.height * 1.05f) + ((gc.currentWindowHeight - ga.myBgTextureOverlay.height) / 2.0f), ga.myBgTextureOverlay.width, ga.puzzleImage3Texture.height };
		
		// DISPLAY EVERYTHING HERE NOW
		BeginDrawing();
		ClearBackground(BLACK);

		DrawTexture(ga.myBgTexture, (gc.currentWindowWidth - ga.myBgTexture.width) / 2, (gc.currentWindowHeight - ga.myBgTexture.height) / 2, RAYWHITE);

		DrawText("Press [I] to import an image", 10, 10, 20, DARKGRAY);

		switch (gc.currentScene)
		{
			case Scene::MENU_SCENE: 
			{
				gA::startButton.draw({ (gc.currentWindowWidth / 2.0f) - (gA::startButton.getButtonWidth() / 2.0f), (gc.currentWindowHeight / 1.9f) }, gc.clickLocation);
				gA::exitButton.draw({ (gc.currentWindowWidth / 2.0f) - (gA::exitButton.getButtonWidth() / 2.0f), (gc.currentWindowHeight / 1.9f) + (gA::startButton.getButtonHeight() * 1.1f) }, gc.clickLocation);
			} break;

			case Scene::CHOOSE_IMAGE_SCENE: 
			{
				DrawTexture(ga.myBgTextureOverlay, (gc.currentWindowWidth - ga.myBgTextureOverlay.width) / 2, (gc.currentWindowHeight - ga.myBgTextureOverlay.height) / 2, WHITE);
				gA::backButton.draw({ (gc.currentWindowWidth - ga.myBgTexture.width) / 2 + 10.0f, (gc.currentWindowHeight - ga.myBgTexture.height) + 10.0f }, gc.clickLocation);

				DrawTexture(ga.txt_ChooseImage_texture, (ga.myBgTextureOverlay.width / 2) - (ga.txt_ChooseImage_texture.width / 2) + ((gc.currentWindowWidth - ga.myBgTextureOverlay.width) / 2), ((gc.currentWindowHeight - ga.myBgTextureOverlay.height) / 2) - (ga.txt_ChooseImage_texture.height / 2), WHITE);

				if (gc.clickLocation.x >= puz1ImgLoc.x && gc.clickLocation.x < (puz1ImgLoc.x + puz1ImgLoc.width) && gc.clickLocation.y >= puz1ImgLoc.y && gc.clickLocation.y < (puz1ImgLoc.y + ga.puzzleImage1Texture.height)) {
					DrawRectangle((gc.currentWindowWidth - ga.myBgTextureOverlay.width) / 2, puz1ImgLoc.y, ga.myBgTextureOverlay.width, ga.puzzleImage1Texture.height, YELLOW);
					gc.puz1hover = true;
				}
				else if (gc.clickLocation.x >= puz2ImgLoc.x && gc.clickLocation.x < (puz2ImgLoc.x + puz2ImgLoc.width) && gc.clickLocation.y >= puz2ImgLoc.y && gc.clickLocation.y < (puz2ImgLoc.y + ga.puzzleImage2Texture.height)) {
					DrawRectangle((gc.currentWindowWidth - ga.myBgTextureOverlay.width) / 2, puz2ImgLoc.y, ga.myBgTextureOverlay.width, ga.puzzleImage2Texture.height, YELLOW);
					gc.puz2hover = true;
				}
				else if (gc.clickLocation.x >= puz3ImgLoc.x && gc.clickLocation.x < (puz3ImgLoc.x + puz3ImgLoc.width) && gc.clickLocation.y >= puz3ImgLoc.y && gc.clickLocation.y < (puz3ImgLoc.y + ga.puzzleImage3Texture.height)) {
					DrawRectangle((gc.currentWindowWidth - ga.myBgTextureOverlay.width) / 2, puz3ImgLoc.y, ga.myBgTextureOverlay.width, ga.puzzleImage3Texture.height, YELLOW);
					gc.puz3hover = true;
				}
				else {
					gc.puz1hover = false;
					gc.puz2hover = false;
					gc.puz3hover = false;
				}
				
				// Icons
				DrawTexture(ga.puzzleImage1Texture, (ga.myBgTextureOverlay.width * 0.1f) + ((gc.currentWindowWidth - ga.myBgTextureOverlay.width) / 2.0f), (ga.myBgTextureOverlay.height * 0.1f) + ((gc.currentWindowHeight - ga.myBgTextureOverlay.height) / 2.0f), WHITE);
				DrawTexture(ga.puzzleImage2Texture, (ga.myBgTextureOverlay.width * 0.1f) + ((gc.currentWindowWidth - ga.myBgTextureOverlay.width) / 2.0f), (ga.myBgTextureOverlay.height * 0.1f) + (ga.puzzleImage1Texture.height * 1.05f) + ((gc.currentWindowHeight - ga.myBgTextureOverlay.height) / 2.0f), WHITE);
				DrawTexture(ga.puzzleImage3Texture, (ga.myBgTextureOverlay.width * 0.1f) + ((gc.currentWindowWidth - ga.myBgTextureOverlay.width) / 2.0f), (ga.myBgTextureOverlay.height * 0.1f) + (ga.puzzleImage1Texture.height * 1.05f) + (ga.puzzleImage2Texture.height * 1.05f) + ((gc.currentWindowHeight - ga.myBgTextureOverlay.height) / 2.0f), WHITE);
				
				// Texts
				DrawTexture(ga.puzImg1Txt_texture, (gc.currentWindowWidth / 2.0f), (ga.myBgTextureOverlay.height * 0.1f) + ((gc.currentWindowHeight - ga.myBgTextureOverlay.height) / 2.0f), BLACK);
				DrawTexture(ga.puzImg2Txt_texture, (gc.currentWindowWidth / 2.0f), (ga.myBgTextureOverlay.height * 0.1f) + (ga.puzzleImage1Texture.height * 1.05f) + ((gc.currentWindowHeight - ga.myBgTextureOverlay.height) / 2.0f), BLACK);
				DrawTexture(ga.puzImg3Txt_texture, (gc.currentWindowWidth / 2.0f), (ga.myBgTextureOverlay.height * 0.1f) + (ga.puzzleImage1Texture.height * 1.05f) + (ga.puzzleImage2Texture.height * 1.05f) + ((gc.currentWindowHeight - ga.myBgTextureOverlay.height) / 2.0f), BLACK);

			} break;

			case Scene::CROP_SLICE_IMAGE_SCENE: 
			{
				gA::backButton.draw({ (gc.currentWindowWidth - ga.myBgTexture.width) / 2 + 10.0f, (gc.currentWindowHeight - ga.myBgTexture.height) + 10.0f }, gc.clickLocation);
				if (ga.myPuzzleTexture.id != 0) {
					// Draw texture in the middle of the screen
					DrawTexture(ga.myPuzzleTexture, (gc.currentWindowWidth - ga.myPuzzleTexture.width) / 2, (gc.currentWindowHeight - ga.myPuzzleTexture.height) / 2, WHITE);
				}
				gc.puz1hover = false;
				gc.puz2hover = false;
				gc.puz3hover = false;
			} break;
		}

		EndDrawing();
	}

	UnloadAssets();


	CloseWindow();

	return 0;
}