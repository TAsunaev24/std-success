#ifndef UI_H_
#define UI_H_

#include <string>
#include <utility>

#include "raylib.h"

namespace theme
{
inline constexpr Color BackgroundTop{10, 12, 21, 255};
inline constexpr Color BackgroundBottom{17, 15, 32, 255};
inline constexpr Color Panel{24, 29, 46, 255};
inline constexpr Color PanelHover{33, 40, 64, 255};
inline constexpr Color PanelBorder{48, 56, 86, 255};
inline constexpr Color Text{234, 238, 252, 255};
inline constexpr Color TextMuted{148, 156, 188, 255};
inline constexpr Color Accent{124, 92, 255, 255};
inline constexpr Color AccentHover{151, 124, 255, 255};
inline constexpr Color Success{70, 214, 160, 255};
inline constexpr Color Gold{255, 199, 92, 255};
inline constexpr Color Floor{18, 22, 34, 255};
inline constexpr Color Wall{49, 56, 86, 255};
inline constexpr Color WallHighlight{65, 74, 112, 255};
inline constexpr Color WallShadow{34, 39, 62, 255};
} // namespace theme

namespace ui
{
void DrawBackground();
void DrawTextCentered(const char *text, float y, float fontSize, Color color, float spacing = 2.0f);
void DrawTextCenteredX(const char *text, float centerX, float y, float fontSize, Color color, float spacing = 2.0f);
void DrawTextRight(const char *text, float rightX, float y, float fontSize, Color color, float spacing = 1.0f);

struct Button
{
	Rectangle rect{0, 0, 0, 0};
	std::string label;
	bool primary = false;
	bool hovered = false;
	bool focused = false;

	Button() = default;
	Button(std::string text, bool isPrimary = false)
		: label(std::move(text)), primary(isPrimary) {}

	void update(Vector2 mouse) { hovered = CheckCollisionPointRec(mouse, rect); }
	bool clicked() const { return hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT); }
	void draw() const;
};

struct ToggleSwitch
{
	Rectangle rect{0, 0, 0, 0};
	bool hovered = false;

	void update(Vector2 mouse) { hovered = CheckCollisionPointRec(mouse, rect); }
	bool clicked() const { return hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT); }
	void draw(bool on) const;
};

// Updates the value while the user drags the slider. Returns true if it changed.
bool UpdateSliderInt(Rectangle track, int &value, int minValue, int maxValue, bool &dragging);
void DrawSliderInt(Rectangle track, int value, int minValue, int maxValue, bool dragging);
} // namespace ui

#endif
