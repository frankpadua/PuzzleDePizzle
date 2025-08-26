#include "Button.h"

Button::Button(Image imageButtonUp, Image imageButtonDown, int bgWidth, int bgHeight, ButtonSize scale)
{
	m_buttonUpImage = imageButtonUp;
	m_buttonDownImage = imageButtonDown;
	m_scale = scale;

	m_buttonUpTexture = LoadTextureFromImage(buttonManipulate(&m_buttonUpImage, bgWidth, bgHeight, scale));
	m_buttonDownTexture = LoadTextureFromImage(buttonManipulate(&m_buttonDownImage, bgWidth, bgHeight, scale));

	//UnloadImage(m_buttonUpImage);
	//UnloadImage(m_buttonDownImage);
}

Button::~Button()
{
	UnloadImage(m_buttonUpImage);
	UnloadImage(m_buttonDownImage);
	UnloadTexture(m_buttonUpTexture);
	UnloadTexture(m_buttonDownTexture);
}

bool Button::isPressed()
{
	if (m_doneClick) {
		m_doneClick = false;
		return true;
	}
	return false;
}

Image Button::buttonManipulate(Image* buttonImage, float bgWidth, float bgHeight, ButtonSize buttonSize) 
{
	int Width = buttonImage->width;
	int Height = buttonImage->height;

	// target size
	float maxWidth{};
	float maxHeight{};

	if (buttonSize == LARGE) {
		maxWidth = bgWidth / 4.0f;
		maxHeight = bgHeight / 4.0f;
	}
	else if (buttonSize == MEDIUM) {
		maxWidth = bgWidth / 7.8f;
		maxHeight = bgHeight / 7.8f;
	}
	else {
		maxWidth = bgWidth / 11.9f;
		maxHeight = bgHeight / 11.9f;
	}

	float scaleX = maxWidth / Width;
	float scaleY = maxHeight / Height;
	float scale = std::min(scaleX, scaleY); // Keep aspect ratio

	auto newWidth = static_cast<int>(Width * scale);
	auto newHeight = static_cast<int>(Height * scale);

	ImageResize(buttonImage, newWidth, newHeight);

	return *buttonImage;
}

void Button::scaled(float bgWidth, float bgHeight, ButtonSize buttonSize)
{
	if (m_buttonUpTexture.id != 0) {
		UnloadTexture(m_buttonUpTexture);
		UnloadTexture(m_buttonDownTexture);
	}

	Image buttonImageTemp = ImageCopy(m_buttonUpImage);
	m_buttonUpTexture = LoadTextureFromImage(buttonManipulate(&buttonImageTemp, bgWidth, bgHeight, buttonSize));
	UnloadImage(buttonImageTemp);
	buttonImageTemp = ImageCopy(m_buttonDownImage);
	m_buttonDownTexture = LoadTextureFromImage(buttonManipulate(&buttonImageTemp, bgWidth, bgHeight, buttonSize));
	UnloadImage(buttonImageTemp);

}

void Button::draw(Vector2 position, Vector2 mousePos)
{
	// Calculate scaled width/height once
	float scaledWidth = m_buttonUpTexture.width * m_scale;
	float scaledHeight = m_buttonUpTexture.height * m_scale;

	Rectangle dest = { position.x, position.y, scaledWidth, scaledHeight };
	Rectangle source = { 0, 0, static_cast<float>(m_buttonUpTexture.width), static_cast<float>(m_buttonUpTexture.height) };

	bool isHovered = CheckCollisionPointRec(mousePos, dest);
	bool isClicked = isHovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON);

	m_doneClick = isClicked ? true : false;

	Texture2D& texture = isClicked ? m_buttonDownTexture : m_buttonUpTexture;

	DrawTexturePro(texture, source, dest, { 0, 0 }, 0.0f, WHITE);
}