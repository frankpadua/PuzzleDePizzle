#include "assets.h"
#include "scene_management.h"
#include "texture_location.h"
#include "transform.h"


#include <iostream>
#include <string_view>
#include <typeinfo>
#include <vector>
#include <raylib.h>

//void theImagePuzzle(Image& myPuzzleImage, const Image& myImageChoosen, Texture& myPuzzleTexture)
//{
//	myPuzzleImage = ImageCopy(myImageChoosen);
//	myPuzzleTexture = LoadTextureFromImage(imageManipulate(&myPuzzleImage, IMAGE_AS_PUZZLE));
//}

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "PuzzleDePizzle");
	
	//SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowMinSize(screenWidth, screenHeight);
	MaximizeWindow();
	SetTargetFPS(60);

	// Initialize all assets
	LoadAssets();
	gA::LoadClassedAssets();

	bool draw_guide{ false };

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

		// Functions and behaviors inside scenes
		sceneFunctions();

		// Resize the texture and maintain quality if the screen dimension is updated
		transformTextures();


		// DISPLAY EVERYTHING HERE NOW
		BeginDrawing();
		ClearBackground(BLACK);

		// The background image
		DrawTexture(ga.myBgTexture, tl.bg.x, tl.bg.y, RAYWHITE);

		switch (gc.currentScene)
		{
			case Scene::MENU_SCENE: 
			{
				gA::playButton.draw({ (gc.currentWindowWidth / 2.0f) - (gA::playButton.getButtonWidth() / 2.0f), (gc.currentWindowHeight / 1.9f) }, gc.clickLocation);
				gA::exitButton.draw({ (gc.currentWindowWidth / 2.0f) - (gA::exitButton.getButtonWidth() / 2.0f), (gc.currentWindowHeight / 1.9f) + (gA::playButton.getButtonHeight() * 1.1f) }, gc.clickLocation);

			} break;

			case Scene::CHOOSE_IMAGE_SCENE: 
			{

				DrawTexture(ga.myBgTextureOverlay, tl.bg_o.x, tl.bg_o.y, WHITE);

				gA::backButton.draw({ (tl.bg.x) * 1.04f, tl.bg.y + 10.0f }, gc.clickLocation);
				gA::folderButton.draw({ (tl.bg.x + tl.bg.width) * 0.9f, tl.bg.y + 10.0f }, gc.clickLocation);


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
				DrawTexture(ga.txt_ChooseImage_texture, tl.bg_o.x + (tl.bg_o.width / 2) - (ga.txt_ChooseImage_texture.width / 2.0f), tl.bg_o.y - (ga.txt_ChooseImage_texture.height / 2.0f), BLACK);
				DrawTexture(ga.puzImg1Txt_texture, (tl.bg_o.x + (tl.bg_o.width / 2.0f)), (tl.icon1.y + (tl.icon1.height / 2.0f)) - (ga.puzImg1Txt_texture.height / 2.0f), BLACK);
				DrawTexture(ga.puzImg2Txt_texture, (tl.bg_o.x + (tl.bg_o.width / 2.0f)), (tl.icon2.y + (tl.icon2.height / 2.0f)) - (ga.puzImg2Txt_texture.height / 2.0f), BLACK);
				DrawTexture(ga.puzImg3Txt_texture, (tl.bg_o.x + (tl.bg_o.width / 2.0f)), (tl.icon3.y + (tl.icon3.height / 2.0f)) - (ga.puzImg3Txt_texture.height / 2.0f), BLACK);

			} break;

			case Scene::CROP_SLICE_IMAGE_SCENE: 
			{
				DrawTexture(ga.myBgTextureOverlay, tl.bg_o.x, tl.bg_o.y, WHITE);
				gA::backButton.draw({ (tl.bg.x) * 1.04f, tl.bg.y + 10.0f }, gc.clickLocation);
				gA::startButton.draw({ (tl.bg.x + tl.bg.width) * 0.9f, tl.bg.y + 10.0f }, gc.clickLocation);

				gA::subButton.draw({ tl.bg_o.x + (tl.bg_o.width / 2.0f) - (gA::subButton.getButtonWidth() + 10.0f), (tl.bg_o.y + tl.bg_o.height)}, gc.clickLocation);
				gA::plusButton.draw({ tl.bg_o.x + (tl.bg_o.width / 2.0f) + 10.0f, (tl.bg_o.y + tl.bg_o.height) }, gc.clickLocation);

				if (ga.myPuzzleTexture.id != 0) {
					// Draw texture in the middle of the screen
					DrawTexture(ga.myPuzzleTexture, tl.p_image.x, tl.p_image.y, WHITE);
				}
				gc.puz1hover = false;
				gc.puz2hover = false;
				gc.puz3hover = false;

				for (const auto& r : sl.rec) {
					DrawRectangleLinesEx(r, 1.0f, GRAY);
				}

			} break;
		}

		EndDrawing();
	}

	UnloadAssets();

	CloseWindow();

	return 0;
}