#pragma once

#include "config.h"
#include "Button.h"

struct Assets {
	// Backgrounds
	Image myBgImage;
	Texture2D myBgTexture;
	Image myBgImageOverlay;
	Texture2D myBgTextureOverlay;

	// Built-in puzzle image
	Image puzzleImage1;
	Texture2D puzzleImage1Texture;
	Image puzzleImage2;
	Texture2D puzzleImage2Texture;
	Image puzzleImage3;
	Texture2D puzzleImage3Texture;

	// Font
	Font myFontLarge;
	Font myFontSmall;

	// Text to Image
	Image txt_ChooseImage;
	Texture2D txt_ChooseImage_texture;
	Image puzImg1Txt;
	Texture2D puzImg1Txt_texture;
	Image puzImg2Txt;
	Texture2D puzImg2Txt_texture;
	Image puzImg3Txt;
	Texture2D puzImg3Txt_texture;

	// THE IMAGE PUZZLE
	Image myPuzzleImage;
	Texture2D myPuzzleTexture;
};

extern Assets ga;

void LoadAssets();
void UnloadAssets();

namespace gA {
	extern Button playButton;
	extern Button exitButton;

	extern Button backButton;
	extern Button folderButton;

	extern Button plusButton;
	extern Button subButton;
	extern Button startButton;

	void LoadClassedAssets();
}