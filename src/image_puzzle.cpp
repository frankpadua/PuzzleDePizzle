#include "image_puzzle.h"

namespace Puzzle
{
    std::vector<Rectangle> arrGrid;
    std::vector<Texture2D> txtTemp;

	std::vector<Texture2D> puzzleTexture;
	std::vector<int> puz_guide;

	int gridSize = 0;
	int blankIndex = -1;
}

void theImagePuzzle(Image& myPuzzleImage, const Image& myImageChoosen, Texture& myPuzzleTexture)
{
	myPuzzleImage = ImageCopy(myImageChoosen);
	myPuzzleTexture = LoadTextureFromImage(imageManipulate(&myPuzzleImage, IMAGE_AS_PUZZLE));
}

bool isSolvable(const std::vector<int>& guide)
{
    int inversions = 0;

    // Count inversions
    for (int i = 0; i < guide.size() - 1; i++) {
        for (int j = i + 1; j < guide.size(); j++) {
            if (guide[i] > guide[j]) inversions++;
        }
    }

    return (inversions % 2 == 0);
}

void randomize(std::vector<Texture2D>& puz, std::vector<int>& guide, int gridSize, int& blankIndex)
{
	std::random_device rd;
	std::mt19937 g(rd());

    while (true) {
        // Shuffle the whole array
	    std::vector<size_t> indices(puz.size() - 1);
	    std::iota(indices.begin(), indices.end(), 0);
	    std::shuffle(indices.begin(), indices.end(), g);

	    std::vector<Texture2D> new_puz;
	    std::vector<int> new_guide;
        new_puz.reserve(indices.size());
        new_guide.reserve(indices.size());

	    for (const auto& e : indices) {
            new_puz.push_back(puz[e]);
            new_guide.push_back(guide[e]);
	    }

        if (isSolvable(new_guide)) {
            // Then place the blankIndex at the very last
            new_puz.push_back(puz.back());
            new_guide.push_back(guide.back());
            // Then replace the original
	        puz.swap(new_puz);
	        guide.swap(new_guide);
            break;
        }
    }
}

// Initial load
void Puzzle::loadPuzzle(const Image& puzImage, const std::vector<Rectangle>& rec)
{
    puzzleTexture.clear();
    puz_guide.clear();

	puzzleTexture.reserve(std::size(rec));
	puz_guide.reserve(std::size(puzzleTexture));
    
    // Refresh image transform
    Image tmp = ImageCopy(puzImage);
    Image tmp2 = imageManipulate(&tmp, IMAGE_AS_PUZZLE);

    int i = 1;
	for (const auto& r : rec) {
		puzzleTexture.push_back(LoadTextureFromImage(ImageFromImage(tmp2, r)));
		puz_guide.push_back(i);
		++i;
        arrGrid.push_back(r);
	}

    gridSize = (int)std::lround(std::sqrt((double)rec.size()));
    blankIndex = (int)puz_guide.size() - 1;

	randomize(puzzleTexture, puz_guide, gridSize, blankIndex);
}

// Final load
void Puzzle::finalizeLoad()
{
    const auto total = (int)puzzleTexture.size();
    auto n = (int)std::lround(std::sqrt((double)total));
    if (n * n != total) {
        // handle error: textures must form a square grid
        TraceLog(LOG_WARNING, "Puzzle::finalizeLoad: not a perfect square: total=%d", total);
    }
    gridSize = n;
    blankIndex = total - 1; // last tile initially blank (matches your randomize)
}

bool Puzzle::tryMove(int dr, int dc)
{
    if (gridSize <= 0 || blankIndex < 0) return false;

    auto [r, c] = toRowCol(blankIndex);
    int nr = r + dr;
    int nc = c + dc;

    // bounds check (signed ints — avoids underflow)
    if (nr < 0 || nr >= gridSize || nc < 0 || nc >= gridSize) return false;

    int swapIndex = toIndex(nr, nc);

    // swap both texture and guide so they remain in sync
    std::swap(puzzleTexture[blankIndex], puzzleTexture[swapIndex]);
    std::swap(puz_guide[blankIndex], puz_guide[swapIndex]);

    blankIndex = swapIndex;
    return true;
}
