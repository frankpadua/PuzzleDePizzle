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
private:
	Image m_buttonUpImage{ nullptr };
	Image m_buttonDownImage{ nullptr };
	Texture2D m_buttonUpTexture{ 0 };
	Texture2D m_buttonDownTexture{ 0 };
	Vector2 m_position{ 0 };
	ButtonSize m_scale;
	bool m_doneClick{ false };

public:
	Button(Image imageButtonUp, Image imageButtonDown, int bgWidth, int bgHeight, ButtonSize scale);
	~Button();

	void draw(Vector2 position, Vector2 mousePos);
	float getButtonWidth() const { return m_buttonUpTexture.width; }
	float getButtonHeight() const { return m_buttonUpTexture.height; }
	int getButtonId() const { return m_buttonUpTexture.id; }
	void unloadButtonTexture() { UnloadTexture(m_buttonUpTexture); UnloadTexture(m_buttonDownTexture); }
	bool isPressed();
	Image buttonManipulate(Image* buttonImage, float bgWidth, float bgHeight, ButtonSize buttonSize);
	void scaled(float bgWidth, float bgHeight, ButtonSize buttonSize);
};