#include "texture_location.h"
#include "transform.h"

Image imageManipulate(Image* myImage, ImageType imageType)
{
	// Screen Width/Height
	int gsw = GetScreenWidth();
	int gsh = GetScreenHeight();

	// Get image dimensions
	int Width = myImage->width;
	int Height = myImage->height;

	// target size
	float maxWidth{};
	float maxHeight{};
	// Compute scale ratio while preserving aspect ratio
	float scaleX{};
	float scaleY{};
	float scale{};
	// New dimensions
	int newWidth{};
	int newHeight{};

	if (imageType == IMAGE_AS_ICON) {
		maxWidth = ga.myBgTexture.width / 5.0f;
		maxHeight = ga.myBgTexture.height / 5.0f;
		scaleX = maxWidth / Width;
		scaleY = maxHeight / Height;
		scale = std::min(scaleX, scaleY);

		newWidth = static_cast<int>(Width * scale);
		newHeight = static_cast<int>(Height * scale);
	}
	else if (imageType == IMAGE_AS_BG_OVERLAY) {
		maxWidth = gsw / 1.3f;
		maxHeight = gsh / 1.3f;
		scaleX = maxWidth / Width;
		scaleY = maxHeight / Height;
		scale = std::min(scaleX, scaleY);

		newWidth = static_cast<int>(Width * scale);
		newHeight = static_cast<int>(Height * scale);
	}
	else if (imageType == IMAGE_AS_PUZZLE) {
		maxWidth = gsw / 1.7f;
		maxHeight = gsh / 1.7f;
		scaleX = maxWidth / Width;
		scaleY = maxHeight / Height;
		scale = std::min(scaleX, scaleY);

		newWidth = static_cast<int>(Width * scale);
		newHeight = static_cast<int>(Height * scale);
	}
	else {
		maxWidth = gsw;
		maxHeight = gsh;
		scaleX = maxWidth / Width;
		scaleY = maxHeight / Height;
		scale = std::min(scaleX, scaleY);

		newWidth = static_cast<int>(Width * scale);
		newHeight = static_cast<int>(Height * scale);
	}

	ImageResize(myImage, newWidth, newHeight);

	std::cout << "THE IMAGE IS: " << myImage->width << 'x' << myImage->height << '\n';

	return *myImage;
}

void textureTransform(const Image& image, Texture2D& texture, ImageType imageType)
{
	if (image.data != nullptr) {
		if (texture.id != 0) {
			UnloadTexture(texture);
		}

		Image temp = ImageCopy(image);
		texture = LoadTextureFromImage(imageManipulate(&temp, imageType));
		UnloadImage(temp);
	}
}

void transformTextures()
{
		// Resize the texture and maintain quality if the screen dimension is updated
		if (gc.currentWindowWidth != GetScreenWidth() || gc.currentWindowHeight != GetScreenHeight())
		{
			// The choosed image
			textureTransform(ga.myPuzzleImage, ga.myPuzzleTexture, IMAGE_AS_PUZZLE);

			// BG and BG Overlay
			textureTransform(ga.myBgImage, ga.myBgTexture, IMAGE_AS_BG);
			textureTransform(ga.myBgImageOverlay, ga.myBgTextureOverlay, IMAGE_AS_BG_OVERLAY);

			// The built-in puzzle images
			textureTransform(ga.puzzleImage1, ga.puzzleImage1Texture, IMAGE_AS_ICON);
			textureTransform(ga.puzzleImage2, ga.puzzleImage2Texture, IMAGE_AS_ICON);
			textureTransform(ga.puzzleImage3, ga.puzzleImage3Texture, IMAGE_AS_ICON);

			// Text as images
			// Choose Image Scene
			textureTransform(ga.txt_ChooseImage, ga.txt_ChooseImage_texture, IMAGE_AS_ICON);
			textureTransform(ga.puzImg1Txt, ga.puzImg1Txt_texture, IMAGE_AS_ICON);
			textureTransform(ga.puzImg2Txt, ga.puzImg2Txt_texture, IMAGE_AS_ICON);
			textureTransform(ga.puzImg3Txt, ga.puzImg3Txt_texture, IMAGE_AS_ICON);

			// Buttons
			gA::playButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, LARGE);
			gA::exitButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, LARGE);

			gA::backButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, MEDIUM);
			gA::folderButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, MEDIUM);

			gA::plusButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, SMALL);
			gA::subButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, SMALL);
			gA::startButton.scaled(ga.myBgTexture.width, ga.myBgTexture.height, MEDIUM);

			// Then assign to current window resolution
			gc.currentWindowWidth = GetScreenWidth();
			gc.currentWindowHeight = GetScreenHeight();
		}
}