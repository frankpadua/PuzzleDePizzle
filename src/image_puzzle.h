#pragma once

#include "assets.h"
#include "transform.h"
#include <random>
#include <raylib.h>
#include <vector>

namespace Puzzle 
{
	extern std::vector<Texture2D> puzzleTexture;

	void loadPuzzle(const Image& puzImage, const std::vector<Rectangle>& rec);
}

void theImagePuzzle(Image& myPuzzleImage, const Image& myImageChoosen, Texture& myPuzzleTexture);