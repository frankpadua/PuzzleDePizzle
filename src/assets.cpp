#include "assets.h"
#include "Button.h"
#include <iostream>

Assets ga{};

namespace gA {
	Button startButton;
	Button exitButton;
	Button backButton;
}

void LoadAssets()
{
	// From Config
	gc.currentScene = Scene::MENU_SCENE;
	gc.puz1hover = false;
	gc.puz2hover = false;
	gc.puz3hover = false;

	// Backgrounds
	ga.myBgImage = LoadImage("Resources/Images/bg.png");
	ga.myBgTexture = LoadTextureFromImage(ga.myBgImage);
	ga.myBgImageOverlay = LoadImage("Resources/Images/bgOverlay.png");
	ga.myBgTextureOverlay = LoadTextureFromImage(ga.myBgImageOverlay);

	// Built-in puzzle image
	ga.puzzleImage1 = LoadImage("Resources/Images/bertface.png");
	ga.puzzleImage1Texture = LoadTextureFromImage(ga.puzzleImage1);
	ga.puzzleImage2 = LoadImage("Resources/Images/jerry.jpg");
	ga.puzzleImage2Texture = LoadTextureFromImage(ga.puzzleImage2);
	ga.puzzleImage3 = LoadImage("Resources/Images/mayon.jpg");
	ga.puzzleImage3Texture = LoadTextureFromImage(ga.puzzleImage3);

	// Font
	ga.myFontLarge = LoadFontEx("Resources/Font/The Bomb Sound.ttf", fontSizeLarge, nullptr, 0);
	ga.myFontSmall = LoadFontEx("Resources/Font/The Bomb Sound.ttf", fontSizeSmall, nullptr, 0);

	// Text to Image
	ga.txt_ChooseImage = ImageTextEx(ga.myFontLarge, "Choose Image", fontSizeLarge, fontSpacing, BLACK);
	ga.txt_ChooseImage_texture = LoadTextureFromImage(ga.txt_ChooseImage);
	ga.puzImg1Txt = ImageTextEx(ga.myFontLarge, "Bert Face", fontSizeLarge, fontSpacing, BLACK);
	ga.puzImg1Txt_texture = LoadTextureFromImage(ga.puzImg1Txt);
	ga.puzImg2Txt = ImageTextEx(ga.myFontLarge, "High Jerry", fontSizeLarge, fontSpacing, BLACK);
	ga.puzImg2Txt_texture = LoadTextureFromImage(ga.puzImg2Txt);
	ga.puzImg3Txt = ImageTextEx(ga.myFontLarge, "Mayon", fontSizeLarge, fontSpacing, BLACK);
	ga.puzImg3Txt_texture = LoadTextureFromImage(ga.puzImg3Txt);

	// THE IMAGE PUZZLE
	ga.myPuzzleImage;
	ga.myPuzzleTexture;
}

void gA::LoadClassedAssets()
{
	startButton.load(LoadImage("Resources/Images/play_button_up.png"), LoadImage("Resources/Images/play_button_down.png"), MEDIUM);
	exitButton.load(LoadImage("Resources/Images/exit_button_up.png"), LoadImage("Resources/Images/exit_button_down.png"), MEDIUM);
	backButton.load(LoadImage("Resources/Images/back_button_up.png"), LoadImage("Resources/Images/back_button_down.png"), MEDIUM);
}

void UnloadAssets()
{
	UnloadImage(ga.myBgImage);
	UnloadTexture(ga.myBgTexture);
	UnloadImage(ga.myBgImageOverlay);
	UnloadTexture(ga.myBgTextureOverlay);

	UnloadImage(ga.puzzleImage1);
	UnloadTexture(ga.puzzleImage1Texture);
	UnloadImage(ga.puzzleImage2);
	UnloadTexture(ga.puzzleImage2Texture);
	UnloadImage(ga.puzzleImage3);
	UnloadTexture(ga.puzzleImage3Texture);

	UnloadImage(ga.myPuzzleImage);
	UnloadTexture(ga.myPuzzleTexture);

	UnloadImage(ga.txt_ChooseImage);
	UnloadTexture(ga.txt_ChooseImage_texture);

	UnloadImage(ga.puzImg1Txt);
	UnloadTexture(ga.puzImg1Txt_texture);
	UnloadImage(ga.puzImg2Txt);
	UnloadTexture(ga.puzImg2Txt_texture);
	UnloadImage(ga.puzImg3Txt);
	UnloadTexture(ga.puzImg3Txt_texture);

	UnloadFont(ga.myFontLarge);
	UnloadFont(ga.myFontSmall);


	// Classed Assets
	gA::startButton.unloadImage();
	gA::startButton.unloadTexture();
	gA::exitButton.unloadImage();
	gA::exitButton.unloadTexture();
	gA::backButton.unloadImage();
	gA::backButton.unloadTexture();
}