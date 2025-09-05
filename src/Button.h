#pragma once

#include <algorithm>
#include <iostream>
#include <raylib.h>

enum ButtonSize {
	SMALL,
	MEDIUM,
	LARGE
};

class Button {
public:
	Button() = default;
	Button(Image imageButtonUp, Image imageButtonDown, ButtonSize scale);

	Button(Button&& other) noexcept;
	Button& operator=(Button&& other) noexcept;

	~Button();

	void draw(Vector2 position, Vector2 mousePos);

	float getButtonWidth() const { return m_buttonUpTexture.width; }
	float getButtonHeight() const { return m_buttonUpTexture.height; }

	bool isPressed();
	Image buttonManipulate(Image* buttonImage, float bgWidth, float bgHeight, ButtonSize buttonSize);
	void scaled(float bgWidth, float bgHeight, ButtonSize buttonSize);

	void load(Image imageButtonUp, Image imageButtonDown, ButtonSize scale);
	void unloadImage();
	void unloadTexture();
	void reset();

private:
	Image m_buttonUpImage{ nullptr };
	Image m_buttonDownImage{ nullptr };
	Texture2D m_buttonUpTexture{ 0 };
	Texture2D m_buttonDownTexture{ 0 };

	ButtonSize m_scale{ SMALL };
	Vector2 m_position{ 0 };
	bool m_doneClick{ false };
};