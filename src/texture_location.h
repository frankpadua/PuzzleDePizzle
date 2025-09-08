#pragma once

#include "assets.h"
#include "config.h"
#include <raylib.h>

struct TextureLoc
{
	// Background
	Rectangle bg;
	// Background overlay
	Rectangle bg_o;

	// Icons
	Rectangle icon1;
	Rectangle icon2;
	Rectangle icon3;
	
	// Other shapes
	Rectangle icon1sel;
	Rectangle icon2sel;
	Rectangle icon3sel;

	// Buttons
	Rectangle st_b;
};

void getShapes();

extern TextureLoc tl;
