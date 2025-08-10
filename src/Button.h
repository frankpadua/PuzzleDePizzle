#pragma once
#include <algorithm>
#include <raylib.h>

class Button {
private:
	Image m_buttonUpImage{ nullptr };
	Image m_buttonDownImage{ nullptr };
	Texture2D m_buttonUpTexture{ 0 };
	Texture2D m_buttonDownTexture{ 0 };
	Vector2 m_position{ 0 };
	float m_scale{ 0.0f };
	bool m_doneClick{ false };

public:
	Button(Image imageButtonUp, Image imageButtonDown, float scale);
	~Button();

	void draw(Vector2 position, Vector2 mousePos);
	float getButtonWidth() const { return m_buttonUpTexture.width * m_scale; }
	float getButtonHeight() const { return m_buttonUpTexture.height * m_scale; }
	int getButtonId() const { return m_buttonUpTexture.id; }
	void unloadButtonTexture() { UnloadTexture(m_buttonUpTexture); UnloadTexture(m_buttonDownTexture); }
	bool isPressed();
	void scaled(float bgWidth, float bgHeight);
};