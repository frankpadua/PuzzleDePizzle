#include "image_manipulate.h"

//Image imageManipulate(Image* myImage, ImageType imageType)
//{
//	int Width = myImage->width;
//	int Height = myImage->height;
//
//	// target size
//	float maxWidth{};
//	float maxHeight{};
//	// Compute scale ratio while preserving aspect ratio
//	float scaleX{};
//	float scaleY{};
//	float scale{};
//	// New dimensions
//	int newWidth{};
//	int newHeight{};
//
//	if (imageType == IMAGE_AS_ICON) {
//		maxWidth = GetScreenWidth() / 5.0f;
//		maxHeight = GetScreenHeight() / 5.0f;
//		scaleX = maxWidth / Width;
//		scaleY = maxHeight / Height;
//		scale = std::min(scaleX, scaleY);
//
//		newWidth = static_cast<int>(Width * scale);
//		newHeight = static_cast<int>(Height * scale);
//	}
//	else if (imageType == IMAGE_AS_BG_OVERLAY) {
//		maxWidth = GetScreenWidth() / 1.3f;
//		maxHeight = GetScreenHeight() / 1.3f;
//		scaleX = maxWidth / Width;
//		scaleY = maxHeight / Height;
//		scale = std::min(scaleX, scaleY);
//
//		newWidth = static_cast<int>(Width * scale);
//		newHeight = static_cast<int>(Height * scale);
//	}
//	else if (imageType == IMAGE_AS_PUZZLE) {
//		maxWidth = GetScreenWidth() / 1.1f;
//		maxHeight = GetScreenHeight() / 1.1f;
//		scaleX = maxWidth / Width;
//		scaleY = maxHeight / Height;
//		scale = std::min(scaleX, scaleY);
//
//		newWidth = static_cast<int>(Width * scale);
//		newHeight = static_cast<int>(Height * scale);
//	}
//	else {
//		maxWidth = GetScreenWidth();
//		maxHeight = GetScreenHeight();
//		scaleX = maxWidth / Width;
//		scaleY = maxHeight / Height;
//		scale = std::min(scaleX, scaleY);
//
//		newWidth = static_cast<int>(Width * scale);
//		newHeight = static_cast<int>(Height * scale);
//	}
//
//	ImageResize(myImage, newWidth, newHeight);
//
//	return *myImage;
//}