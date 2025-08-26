#pragma once
#include <raylib.h>

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
	IMAGE_AS_FONT,
	IMAGE_AS_ICON,
	IMAGE_AS_PUZZLE
};

inline namespace Assets
{
	inline constexpr int screenHeight{ 720 };
	inline constexpr int screenWidth{ 1280 };
}