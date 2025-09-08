#include "assets.h"
#include "ImageCrop.h"
//#include "Random.h"
#include "texture_location.h"
#include "transform.h"

#define TINYFILEDIALOGS_IMPLEMENTATION
#include "FileDialog/tinyfiledialogs.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>
#include <typeinfo>
#include <vector>
#include <raylib.h>

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

	bool draw_guide{ false };

	float sliceX = 0;
	float sliceY = 0;
	float sliceWidth = 100;
	float sliceHeight = 100;

	Rectangle tile = {
		sliceX,
		sliceY,
		sliceWidth,
		sliceHeight
	};

	std::vector<Rectangle> rec3b3{};
	rec3b3.reserve(9);
	bool doneDrawing = false;

	for (int col{ 0 }; col < 3; ++col) {
		for (int row{ 0 }; row < 3; ++row) {
			rec3b3.emplace_back(tile);
			tile.x += tile.width;
		}
		tile.x = 0;
		tile.y += tile.height;
	}

	while (!WindowShouldClose())
	{
		// MouseClickLocation
		gc.clickLocation = GetMousePosition();

		// Get the texture shapes and location
		getShapes();

		// LOCKING/UNLOCKING RESIZE WINDOW
		if (IsKeyPressed(KEY_R))
		{
			if (IsWindowState(FLAG_WINDOW_RESIZABLE)) {
				ClearWindowState(FLAG_WINDOW_RESIZABLE);
			}
			else SetWindowState(FLAG_WINDOW_RESIZABLE);
		}
		if (IsKeyPressed(KEY_G))
		{
			if (draw_guide)		draw_guide = false;
			else                draw_guide = true;
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
			// The choosed image
			textureTransform(ga.myPuzzleImage, ga.myPuzzleTexture, IMAGE_AS_PUZZLE);

			// BG and BG Overlay
			textureTransform(ga.myBgImage, ga.myBgTexture, IMAGE_AS_BG);
			textureTransform(ga.myBgImageOverlay, ga.myBgTextureOverlay, IMAGE_AS_BG_OVERLAY);

			// The built-in puzzle images
			textureTransform(ga.puzzleImage1, ga.puzzleImage1Texture, IMAGE_AS_ICON);
			textureTransform(ga.puzzleImage2, ga.puzzleImage2Texture, IMAGE_AS_ICON);
			textureTransform(ga.puzzleImage3, ga.puzzleImage3Texture, IMAGE_AS_ICON);

			// Text as images
			// Choose Image Scene
			textureTransform(ga.txt_ChooseImage, ga.txt_ChooseImage_texture, IMAGE_AS_ICON);
			textureTransform(ga.puzImg1Txt, ga.puzImg1Txt_texture, IMAGE_AS_ICON);
			textureTransform(ga.puzImg2Txt, ga.puzImg2Txt_texture, IMAGE_AS_ICON);
			textureTransform(ga.puzImg3Txt, ga.puzImg3Txt_texture, IMAGE_AS_ICON);

			// Buttons
			gA::startButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, LARGE);
			gA::exitButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, LARGE);
			gA::backButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, MEDIUM);

			// Then assign to current window resolution
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


		// DISPLAY EVERYTHING HERE NOW
		BeginDrawing();
		ClearBackground(BLACK);

		// The background image
		DrawTexture(ga.myBgTexture, tl.bg.x, tl.bg.y, RAYWHITE);

		DrawText("Press [I] to import an image", 10, 10, 20, DARKGRAY);


		for (const auto& r : rec3b3) {
			DrawRectangleLinesEx(r, 4.0f, BLUE);
		}

		switch (gc.currentScene)
		{
			case Scene::MENU_SCENE: 
			{
				gA::startButton.draw({ (gc.currentWindowWidth / 2.0f) - (gA::startButton.getButtonWidth() / 2.0f), (gc.currentWindowHeight / 1.9f) }, gc.clickLocation);
				gA::exitButton.draw({ (gc.currentWindowWidth / 2.0f) - (gA::exitButton.getButtonWidth() / 2.0f), (gc.currentWindowHeight / 1.9f) + (gA::startButton.getButtonHeight() * 1.1f) }, gc.clickLocation);

				if (draw_guide) {
					DrawRectangleLinesEx(tl.bg, 10.0f, RED);
				}
			} break;

			case Scene::CHOOSE_IMAGE_SCENE: 
			{

				DrawTexture(ga.myBgTextureOverlay, tl.bg_o.x, tl.bg_o.y, WHITE);
				gA::backButton.draw({ (gc.currentWindowWidth - ga.myBgTexture.width) / 2 + 10.0f, tl.bg.y + 10.0f }, gc.clickLocation);

				DrawTexture(ga.txt_ChooseImage_texture, tl.bg_o.x + (tl.bg_o.width / 2) - (ga.txt_ChooseImage_texture.width / 2.0f), tl.bg_o.y - (ga.txt_ChooseImage_texture.height / 2.0f), WHITE);

				if (gc.clickLocation.x >= tl.icon1sel.x && gc.clickLocation.x < (tl.icon1sel.x + tl.icon1sel.width) && gc.clickLocation.y >= tl.icon1sel.y && gc.clickLocation.y < (tl.icon1.y + tl.icon1sel.height)) {
					DrawRectangleRec(tl.icon1sel, SKYBLUE);
					gc.puz1hover = true;
				}
				else if (gc.clickLocation.x >= tl.icon2sel.x && gc.clickLocation.x < (tl.icon2sel.x + tl.icon2sel.width) && gc.clickLocation.y >= tl.icon2sel.y && gc.clickLocation.y < (tl.icon2.y + tl.icon2sel.height)) {
					DrawRectangleRec(tl.icon2sel, YELLOW);
					gc.puz2hover = true;
				}
				else if (gc.clickLocation.x >= tl.icon3sel.x && gc.clickLocation.x < (tl.icon3sel.x + tl.icon3sel.width) && gc.clickLocation.y >= tl.icon3sel.y && gc.clickLocation.y < (tl.icon3.y + tl.icon3sel.height)) {
					DrawRectangleRec(tl.icon3sel, GREEN);
					gc.puz3hover = true;
				}
				else {
					gc.puz1hover = false;
					gc.puz2hover = false;
					gc.puz3hover = false;
				}
				
				// Icons
				DrawTexture(ga.puzzleImage1Texture, tl.icon1.x, tl.icon1.y, WHITE);
				DrawTexture(ga.puzzleImage2Texture, tl.icon3.x, tl.icon2.y, WHITE);
				DrawTexture(ga.puzzleImage3Texture, tl.icon3.x, tl.icon3.y, WHITE);
				
				// Texts
				DrawTexture(ga.puzImg1Txt_texture, (tl.bg_o.x + (tl.bg_o.width / 2.0f)), (tl.icon1.y + (tl.icon1.height / 2.0f)) - (ga.puzImg1Txt_texture.height / 2.0f), BLACK);
				DrawTexture(ga.puzImg2Txt_texture, (tl.bg_o.x + (tl.bg_o.width / 2.0f)), (tl.icon2.y + (tl.icon2.height / 2.0f)) - (ga.puzImg2Txt_texture.height / 2.0f), BLACK);
				DrawTexture(ga.puzImg3Txt_texture, (tl.bg_o.x + (tl.bg_o.width / 2.0f)), (tl.icon3.y + (tl.icon3.height / 2.0f)) - (ga.puzImg3Txt_texture.height / 2.0f), BLACK);

				if (draw_guide) {
					DrawRectangleLinesEx(tl.bg_o, 3.0f, RED);

					DrawRectangleLinesEx(tl.icon1, 3.0f, RED);
					DrawRectangleLinesEx(tl.icon1sel, 3.0f, RED);
					DrawRectangleLinesEx(tl.icon3sel, 3.0f, RED);
				}
			} break;

			case Scene::CROP_SLICE_IMAGE_SCENE: 
			{
				gA::backButton.draw({ (gc.currentWindowWidth - ga.myBgTexture.width) / 2 + 10.0f, tl.bg.y + 10.0f }, gc.clickLocation);
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