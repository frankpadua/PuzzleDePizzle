#include "scene_management.h"

void sceneFunctions()
{
		// SCENE 1
		if (gc.currentScene == Scene::MENU_SCENE) {
			// Scene 1-2 switch
			if (gA::playButton.isPressed()) {
				gc.currentScene = Scene::CHOOSE_IMAGE_SCENE;
			}
		}
		// SCENE 2
		if (gc.currentScene == Scene::CHOOSE_IMAGE_SCENE) {
			// Scene 2-1 switch
			if (gA::backButton.isPressed()) {
				gc.currentScene = Scene::MENU_SCENE;
			}
			// Scene 2-3 switch
			if (gc.puz1hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				theImagePuzzle(ga.myPuzzleImage, ga.puzzleImage1, ga.myPuzzleTexture);
				gc.currentScene = Scene::CROP_SLICE_IMAGE_SCENE;
			}
			if (gc.puz2hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				theImagePuzzle(ga.myPuzzleImage, ga.puzzleImage2, ga.myPuzzleTexture);
				gc.currentScene = Scene::CROP_SLICE_IMAGE_SCENE;
			}
			if (gc.puz3hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				theImagePuzzle(ga.myPuzzleImage, ga.puzzleImage3, ga.myPuzzleTexture);
				gc.currentScene = Scene::CROP_SLICE_IMAGE_SCENE;
			}
			if (gA::folderButton.isPressed()) {
				const char* filepath = tinyfd_openFileDialog("Select an Image",	"",	3, FILTER, "Image files", 0);

				if (filepath) {
					if (ga.myPuzzleTexture.id != 0) {
						UnloadTexture(ga.myPuzzleTexture);
					}

					ga.myPuzzleImage = LoadImage(filepath); // Load image
					ga.myPuzzleTexture = LoadTextureFromImage(imageManipulate(&ga.myPuzzleImage, IMAGE_AS_PUZZLE));
				}
				gc.currentScene = Scene::CROP_SLICE_IMAGE_SCENE;
			}
		}
		// SCENE 3
		if (gc.currentScene == Scene::CROP_SLICE_IMAGE_SCENE) {
			// Scene 3-2 switch
			if (gA::backButton.isPressed()) {
				if (ga.myPuzzleTexture.id != 0) {
					UnloadTexture(ga.myPuzzleTexture);
				}
				gc.currentScene = Scene::CHOOSE_IMAGE_SCENE;
			}
			// Scene 3 actions
			if (gA::plusButton.isPressed()) {
				sl.slice++;
			}
			if (gA::subButton.isPressed()) {
				sl.slice--;
			}
			// Slice the image
			sl.applySlice();
		}
}