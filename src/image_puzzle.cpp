#include "image_puzzle.h"

namespace Puzzle
{
	std::vector<Texture2D> puzzleTexture;
}

void randomize(std::vector<Texture2D>& puz)
{
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(puz.begin(), puz.end() - 1, g);
}

void Puzzle::loadPuzzle(const Image& puzImage, const std::vector<Rectangle>& rec)
{
	puzzleTexture.reserve(std::size(rec));

	for (const auto& r : rec)
		puzzleTexture.push_back(LoadTextureFromImage(ImageFromImage(puzImage, r)));

	randomize(puzzleTexture);
}

void theImagePuzzle(Image& myPuzzleImage, const Image& myImageChoosen, Texture& myPuzzleTexture)
{
	myPuzzleImage = ImageCopy(myImageChoosen);
	myPuzzleTexture = LoadTextureFromImage(imageManipulate(&myPuzzleImage, IMAGE_AS_PUZZLE));
}