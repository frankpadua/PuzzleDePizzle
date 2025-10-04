#include "assets.h"
#include "image_puzzle.h"
#include "scene_management.h"
#include "texture_location.h"
#include "transform.h"

#include <iostream>
#include <string>
#include <string_view>
#include <typeinfo>
#include <vector>
#include <raylib.h>

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
		Vector2 livemouse = GetMouseDelta();
		std::string mouseX = std::to_string(gc.clickLocation.x);
		std::string mouseY = std::to_string(gc.clickLocation.y);
		const char* mX = mouseX.c_str();
		const char* mY = mouseY.c_str();

		// Resize the texture and maintain quality if the screen dimension is updated
		transformTextures();
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

		if (gc.currentScene == Scene::BEGIN_PLAY_SCENE)
		{
			// Reverse input
			// W = down, S = up, A = right, D = left
			if (IsKeyPressed(KEY_S)) { Puzzle::tryMove(-1, 0); } // move blank up
			if (IsKeyPressed(KEY_W)) { Puzzle::tryMove( 1, 0); } // move blank down
			if (IsKeyPressed(KEY_D)) { Puzzle::tryMove( 0,-1); } // move blank left
			if (IsKeyPressed(KEY_A)) { Puzzle::tryMove( 0, 1); } // move blank right
		}

		// Functions and behaviors inside scenes
		sceneFunctions();

		// DISPLAY EVERYTHING HERE NOW
		BeginDrawing();
		ClearBackground(BLACK);

		// The background image
		DrawTexture(ga.myBgTexture, tl.bg.x, tl.bg.y, RAYWHITE);

		DrawText(mX, 0, 0, fontSizeSmall, GRAY);
		DrawText(mY, 0, 30, fontSizeSmall, GRAY);

		switch (gc.currentScene)
		{
			case Scene::MENU_SCENE: 
			{
				gA::playButton.draw({ (gc.currentWindowWidth / 2.0f) - (gA::playButton.getButtonWidth() / 2.0f), 
									(gc.currentWindowHeight / 1.9f) }, gc.clickLocation);
				gA::exitButton.draw({ (gc.currentWindowWidth / 2.0f) - (gA::exitButton.getButtonWidth() / 2.0f), 
									(gc.currentWindowHeight / 1.9f) + (gA::playButton.getButtonHeight() * 1.1f) }, gc.clickLocation);

			} break;

			case Scene::CHOOSE_IMAGE_SCENE: 
			{

				DrawTexture(ga.myBgTextureOverlay, tl.bg_o.x, tl.bg_o.y, WHITE);

				gA::backButton.draw({ (tl.bg.x) * 1.04f, tl.bg.y + 10.0f }, gc.clickLocation);
				gA::folderButton.draw({ (tl.bg.x + tl.bg.width) * 0.9f, tl.bg.y + 10.0f }, gc.clickLocation);

				// FIX THIS SOON
				if (gc.clickLocation.x >= tl.icon1sel.x && gc.clickLocation.x < (tl.icon1sel.x + tl.icon1sel.width) && 
					gc.clickLocation.y >= tl.icon1sel.y && gc.clickLocation.y < (tl.icon1.y + tl.icon1sel.height)) 
				{
					DrawRectangleRec(tl.icon1sel, SKYBLUE);
					gc.puz1hover = true;
				}

				else if (gc.clickLocation.x >= tl.icon2sel.x && gc.clickLocation.x < (tl.icon2sel.x + tl.icon2sel.width) && 
					gc.clickLocation.y >= tl.icon2sel.y && gc.clickLocation.y < (tl.icon2.y + tl.icon2sel.height)) 
				{
					DrawRectangleRec(tl.icon2sel, YELLOW);
					gc.puz2hover = true;
				}

				else if (gc.clickLocation.x >= tl.icon3sel.x && gc.clickLocation.x < (tl.icon3sel.x + tl.icon3sel.width) && 
					gc.clickLocation.y >= tl.icon3sel.y && gc.clickLocation.y < (tl.icon3.y + tl.icon3sel.height)) 
				{
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
				DrawTexture(ga.txt_ChooseImage_texture, 
							tl.bg_o.x + (tl.bg_o.width / 2) - (ga.txt_ChooseImage_texture.width / 2.0f), 
							tl.bg_o.y - (ga.txt_ChooseImage_texture.height / 2.0f), 
							BLACK);
				DrawTexture(ga.puzImg1Txt_texture, 
							(tl.bg_o.x + (tl.bg_o.width / 2.0f)), 
							(tl.icon1.y + (tl.icon1.height / 2.0f)) - (ga.puzImg1Txt_texture.height / 2.0f), 
							BLACK);
				DrawTexture(ga.puzImg2Txt_texture, 
							(tl.bg_o.x + (tl.bg_o.width / 2.0f)), 
							(tl.icon2.y + (tl.icon2.height / 2.0f)) - (ga.puzImg2Txt_texture.height / 2.0f), 
							BLACK);
				DrawTexture(ga.puzImg3Txt_texture, 
							(tl.bg_o.x + (tl.bg_o.width / 2.0f)), 
							(tl.icon3.y + (tl.icon3.height / 2.0f)) - (ga.puzImg3Txt_texture.height / 2.0f), 
							BLACK);

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

			case Scene::BEGIN_PLAY_SCENE:
			{
				DrawTexture(ga.myBgTextureOverlay, tl.bg_o.x, tl.bg_o.y, WHITE);

				for (int i = 0; i < (int)Puzzle::puzzleTexture.size(); ++i) {
				    if (i == Puzzle::blankIndex) continue; // **important**
				    DrawTexture(Puzzle::puzzleTexture[i], sl.rec[i].x, sl.rec[i].y, WHITE);
					// For tile number guide purposes
					if (draw_guide) {
						auto guide_text{
							[](int a) -> const char* {
								static std::string buf;
								buf = std::to_string(a);
								return buf.c_str();
							}
						};
						DrawTextEx(ga.myFontSmall, guide_text(Puzzle::puz_guide[i]), {sl.rec[i].x, sl.rec[i].y}, fontSizeSmall, fontSpacing, BLACK);
					}
				}

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