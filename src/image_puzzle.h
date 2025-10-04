#pragma once

#include "assets.h"
#include "image_slice.h"
#include "transform.h"
#include <numeric>
#include <random>
#include <raylib.h>
#include <vector>

namespace Puzzle 
{
    extern std::vector<Rectangle> arrGrid;
    extern std::vector<Texture2D> txtTemp;

	extern std::vector<Texture2D> puzzleTexture;
	extern std::vector<int> puz_guide;

    extern int gridSize;     // number of columns/rows
    extern int blankIndex; // current blank position (signed int!)

	void loadPuzzle(const Image& puzImage, const std::vector<Rectangle>& rec);

    // helpers
    inline int toIndex(int r, int c) { return r * gridSize + c; }
    inline std::pair<int,int> toRowCol(int idx) { return { idx / gridSize, idx % gridSize }; }

    // call after loading & randomizing textures
    void finalizeLoad(); 

    // movement
    bool tryMove(int dr, int dc); // dr = -1..1, dc = -1..1
}

void theImagePuzzle(Image& myPuzzleImage, const Image& myImageChoosen, Texture& myPuzzleTexture);