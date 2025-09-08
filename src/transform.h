#pragma once

#include "assets.h"
#include "config.h"
#include <raylib.h>

Image imageManipulate(Image* myImage, ImageType imageType);
void textureTransform(const Image& image, Texture2D& texture, ImageType imageType);
