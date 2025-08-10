#include "Button.h"

Button::Button(Image imageButtonUp, Image imageButtonDown, float scale)
{
	m_buttonUpImage = imageButtonUp;
	m_buttonDownImage = imageButtonDown;
	m_scale = scale;

	int originalWidth = m_buttonUpImage.width;
	int originalHeight = m_buttonUpImage.height;

	auto newWidth = static_cast<int>(originalWidth * m_scale);
	auto newHeight = static_cast<int>(originalHeight * m_scale);

	ImageResize(&m_buttonUpImage, newWidth, newHeight);
	ImageResize(&m_buttonDownImage, newWidth, newHeight);

	m_buttonUpTexture = LoadTextureFromImage(m_buttonUpImage);
	m_buttonDownTexture = LoadTextureFromImage(m_buttonDownImage);

	UnloadImage(m_buttonUpImage);
	UnloadImage(m_buttonDownImage);
}

Button::~Button()
{
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

void Button::scaled(float bgWidth, float bgHeight)
{
	auto textureWidth = static_cast<float>(m_buttonUpTexture.width);
	auto textureHeight = static_cast<float>(m_buttonUpTexture.height);

	float maxWidth = bgWidth / 3.7f;
	float maxHeight = bgWidth / 3.7f;

	float scaleX = maxWidth / textureWidth;
	float scaleY = maxHeight / textureHeight;
	m_scale = std::min(scaleX, scaleY); // Keep aspect ratio
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