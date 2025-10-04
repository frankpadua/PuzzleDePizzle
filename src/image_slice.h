#pragma once

#include "texture_location.h"
#include <vector>

#include <raylib.h>

struct Slice {
	int slice{ 3 };
	float prec_slice{};
	std::vector<Rectangle> rec;
	std::vector<Rectangle> puz;
	
	void applySlice();
};

std::vector<Rectangle> GenerateGrid(int cols, int rows, float startX, float startY, float tileWidth, float tileHeight);
void updateSlice();
extern Slice sl;
