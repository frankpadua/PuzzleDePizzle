#include <raylib.h>
#include <raymath.h>

void MyImageCrop() {
	InitWindow(800, 600, "Raylib ROI Selector");
	//ToggleBorderlessWindowed();

	Image image = LoadImage("Resources/bertface.png");
	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image); // Safe after creating texture

	Vector2 startPos = { 0 };
	Vector2 endPos = { 0 };
	bool selecting = false;
	Rectangle selection = { 0 };

	while (!WindowShouldClose()) {
		// Begin selection on mouse press
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			startPos = GetMousePosition();
			selecting = true;
		}

		// Update selection box while dragging
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && selecting) {
			endPos = GetMousePosition();
			selection.x = fminf(startPos.x, endPos.x);
			selection.y = fminf(startPos.y, endPos.y);
			selection.width = fabsf(endPos.x - startPos.x);
			selection.height = fabsf(endPos.y - startPos.y);
		}

		// Finalize selection on release
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && selecting) {
			selecting = false;
			// Now 'selection' has your final ROI rectangle
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawTexture(texture, 0, 0, WHITE);

		// Draw selection box if active
		if (selecting || (selection.width > 0 && selection.height > 0)) {
			DrawRectangleLinesEx(selection, 1, RED);
		}

		EndDrawing();
	}

	UnloadTexture(texture);
	CloseWindow();
}
