#include <algorithm>
#include <iostream>
#include <vector>
#include <raylib.h>

// Initial window size (make fullscreen later)
constexpr float screenHeight{ 800.0f };
constexpr float screenWidth{ 1270.0f };

int main()
{
	InitWindow(screenWidth, screenHeight, "PuzzleDePizzle");
	SetTargetFPS(60);

	// Loading Image and transforming it
	Image image_1 = LoadImage("Resources/bertface.png");
	int minDim = std::min(image_1.width, image_1.height);
	Rectangle cropRect = {
		(image_1.width - minDim) / 2.0f,   // x: center horizontally
		(image_1.height - minDim) / 2.0f,  // y: center vertically
		static_cast<float>(minDim),    // width
		static_cast<float>(minDim)     // height
	};
	ImageCrop(&image_1, cropRect);
	// Fitting to the window (add settings for portrait mode [mobile] later)
	int maxSize = std::min(screenWidth/1.8f, screenHeight/1.8f);
	ImageResize(&image_1, maxSize, maxSize);

	Texture2D texture_1 = LoadTextureFromImage(image_1);


	// Slicing the texture
	int rows = 3; // vertical slices
	int cols = 3; // horizontal slices
	int tileWidth = texture_1.width / cols;
	int tileHeight = texture_1.height / rows;

	std::vector<Rectangle> tileRects;
	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			tileRects.emplace_back(
				static_cast<float>(x * tileWidth),
				static_cast<float>(y * tileHeight),
				static_cast<float>(tileWidth),
				static_cast<float>(tileHeight)
			);
		}
	}

	// Initial value for putting the sliced texture in the center one by one
	// Calculate total width and height of the puzzle grid
	int puzzleWidth = cols * tileWidth;
	int puzzleHeight = rows * tileHeight;
	// Calculate top-left offset to center the grid on screen
	Vector2 puzzleOffset = {
		static_cast<float>((screenWidth - puzzleWidth) / 2),
		static_cast<float>((screenHeight - puzzleHeight) / 2)
	};



	UnloadImage(image_1);


	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		for (int i = 0; i < tileRects.size(); ++i) {
			int row = i / cols;
			int col = i % cols;

			Vector2 screenPos = {
				puzzleOffset.x + static_cast<float>(col * tileWidth),
				puzzleOffset.y + static_cast<float>(row * tileHeight)
			};

			// Draw each slice
			DrawTextureRec(texture_1, tileRects[i], screenPos, WHITE);
			// Draw the border of the slice
			DrawRectangleLines(screenPos.x, screenPos.y, tileRects[i].width, tileRects[i].height, BLACK);
		}



		EndDrawing();
	}



	UnloadTexture(texture_1);

	CloseWindow();

	return 0;
}