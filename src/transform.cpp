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
		maxWidth = gsw / 1.5f;
		maxHeight = gsh / 1.5f;
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