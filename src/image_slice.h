#pragma once

#include "texture_location.h"
#include <vector>

#include <raylib.h>

struct Slice {
	int slice{ 3 };
	std::vector<Rectangle> rec;
	std::vector<Rectangle> puz;

	void applySlice();
};

extern Slice sl;
