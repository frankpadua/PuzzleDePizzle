#include "image_slice.h"

Slice sl{};

// Generate a grid of rectangles
std::vector<Rectangle> GenerateGrid(int cols, int rows, float startX, float startY, float tileWidth, float tileHeight) {
    std::vector<Rectangle> grid;
    grid.reserve(cols * rows);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            grid.push_back({
                startX + col * tileWidth,
                startY + row * tileHeight,
                tileWidth,
                tileHeight
            });
        }
    }
    return grid;
}

void Slice::applySlice()
{
    if (sl.slice < 3) {
        sl.slice = 3;
        return;
    }
    if (sl.slice > 10) {
        sl.slice = 10;
        return;
    }

    // 3 = 0.333f
    if (sl.slice == 3)
        sl.rec = GenerateGrid(3, 3, tl.p_image.x, tl.p_image.y, (tl.p_image.width * 0.33333f), (tl.p_image.height * 0.33333f));
    // 4 = 0.25f
	if (sl.slice == 4)
		sl.rec = GenerateGrid(4, 4, tl.p_image.x, tl.p_image.y, (tl.p_image.width * 0.25f), (tl.p_image.height * 0.25f));
    // 5 = 0.20f
	if (sl.slice == 5)
		sl.rec = GenerateGrid(5, 5, tl.p_image.x, tl.p_image.y, (tl.p_image.width * 0.20f), (tl.p_image.height * 0.20f));
    // 6 = 0.166f
	if (sl.slice == 6)
		sl.rec = GenerateGrid(6, 6, tl.p_image.x, tl.p_image.y, (tl.p_image.width * 0.16666f), (tl.p_image.height * 0.16666f));
    // 7 = 0.1428f
	if (sl.slice == 7)
		sl.rec = GenerateGrid(7, 7, tl.p_image.x, tl.p_image.y, (tl.p_image.width * 0.1428571f), (tl.p_image.height * 0.1428571f));
    // 8 = 0.125f
	if (sl.slice == 8)
		sl.rec = GenerateGrid(8, 8, tl.p_image.x, tl.p_image.y, (tl.p_image.width * 0.125f), (tl.p_image.height * 0.125f));
    // 9 = 0.111f
	if (sl.slice == 9)
		sl.rec = GenerateGrid(9, 9, tl.p_image.x, tl.p_image.y, (tl.p_image.width * 0.11111f), (tl.p_image.height * 0.11111f));
    // 10 = 0.10f
	if (sl.slice == 10)
		sl.rec = GenerateGrid(10, 10, tl.p_image.x, tl.p_image.y, (tl.p_image.width * 0.10f), (tl.p_image.height * 0.10f));
}