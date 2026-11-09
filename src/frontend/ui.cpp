#include "ui.h"

#include <algorithm>
#include <cmath>

namespace
{
constexpr float TextSpacing = 1.5f;

float SliderRatio(int value, int minValue, int maxValue)
{
	if (maxValue <= minValue) return 0.0f;
	return std::clamp(static_cast<float>(value - minValue) / static_cast<float>(maxValue - minValue), 0.0f, 1.0f);
}
} // namespace

namespace ui
{
void DrawBackground()
{
	const int width = GetScreenWidth();
	const int height = GetScreenHeight();
	const float minSide = static_cast<float>(std::min(width, height));

	DrawRectangleGradientV(0, 0, width, height, theme::BackgroundTop, theme::BackgroundBottom);

	DrawCircleGradient({width * 0.12f, height * 0.22f}, minSide * 0.55f,
		Fade(theme::Accent, 0.10f), BLANK);
	DrawCircleGradient({width * 0.92f, height * 0.86f}, minSide * 0.45f,
		Fade(theme::Success, 0.06f), BLANK);

	const int gridStep = 64;
	for (int x = 0; x < width; x += gridStep)
		DrawLine(x, 0, x, height, Fade(WHITE, 0.018f));
	for (int y = 0; y < height; y += gridStep)
		DrawLine(0, y, width, y, Fade(WHITE, 0.018f));
}

void DrawTextCenteredX(const char *text, float centerX, float y, float fontSize, Color color, float spacing)
{
	Vector2 size = MeasureTextEx(GetFontDefault(), text, fontSize, spacing);
	DrawTextEx(GetFontDefault(), text, {centerX - size.x / 2.0f, y}, fontSize, spacing, color);
}

void DrawTextCentered(const char *text, float y, float fontSize, Color color, float spacing)
{
	DrawTextCenteredX(text, GetScreenWidth() / 2.0f, y, fontSize, color, spacing);
}

void DrawTextRight(const char *text, float rightX, float y, float fontSize, Color color, float spacing)
{
	Vector2 size = MeasureTextEx(GetFontDefault(), text, fontSize, spacing);
	DrawTextEx(GetFontDefault(), text, {rightX - size.x, y}, fontSize, spacing, color);
}

void Button::draw() const
{
	constexpr float roundness = 0.24f;
	constexpr int segments = 12;

	Color fill = primary ? theme::Accent : theme::Panel;
	Color border = primary ? theme::AccentHover : theme::PanelBorder;
	Color labelColor = primary ? WHITE : theme::Text;

	if (hovered)
	{
		fill = primary ? theme::AccentHover : theme::PanelHover;
		border = primary ? Color{190, 174, 255, 255} : theme::Accent;
		labelColor = WHITE;
	}
	if (focused)
		border = primary ? WHITE : theme::AccentHover;

	const float pressOffset = (hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ? 2.0f : 0.0f;
	Rectangle body{rect.x, rect.y + pressOffset, rect.width, rect.height};

	DrawRectangleRounded({body.x, body.y + 4.0f, body.width, body.height}, roundness, segments, Fade(BLACK, 0.25f));
	DrawRectangleRounded(body, roundness, segments, fill);
	DrawRectangleRoundedLinesEx(body, roundness, segments, focused ? 2.0f : 1.0f, border);

	const float fontSize = std::clamp(body.height * 0.36f, 12.0f, 26.0f);
	Vector2 measured = MeasureTextEx(GetFontDefault(), label.c_str(), fontSize, TextSpacing);
	Vector2 position{body.x + (body.width - measured.x) / 2.0f,
		body.y + (body.height - measured.y) / 2.0f - 1.0f};
	DrawTextEx(GetFontDefault(), label.c_str(), position, fontSize, TextSpacing, labelColor);
}

void ToggleSwitch::draw(bool on) const
{
	Color track = on ? theme::Accent : theme::PanelBorder;
	if (hovered)
		track = on ? theme::AccentHover : Color{70, 78, 108, 255};

	DrawRectangleRounded(rect, 0.5f, 18, track);
	if (hovered)
		DrawRectangleRoundedLinesEx(rect, 0.5f, 18, 2.0f, Fade(WHITE, 0.55f));

	const float knobRadius = rect.height * 0.5f - 4.0f;
	const float knobX = on ? rect.x + rect.width - rect.height * 0.5f : rect.x + rect.height * 0.5f;
	Vector2 knob{knobX, rect.y + rect.height * 0.5f};

	DrawCircleV({knob.x, knob.y + 1.0f}, knobRadius + 1.0f, Fade(BLACK, 0.25f));
	DrawCircleV(knob, knobRadius, WHITE);
}

bool UpdateSliderInt(Rectangle track, int &value, int minValue, int maxValue, bool &dragging)
{
	Vector2 mouse = GetMousePosition();
	Rectangle hitArea{track.x - 16.0f, track.y - 20.0f, track.width + 32.0f, track.height + 40.0f};

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, hitArea))
		dragging = true;
	if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		dragging = false;

	const int oldValue = value;
	if (dragging && maxValue > minValue && track.width > 0.0f)
	{
		const float ratio = std::clamp((mouse.x - track.x) / track.width, 0.0f, 1.0f);
		const int newValue = minValue + static_cast<int>(std::lround(ratio * (maxValue - minValue)));
		value = std::clamp(newValue, minValue, maxValue);
	}
	return value != oldValue;
}

void DrawSliderInt(Rectangle track, int value, int minValue, int maxValue, bool dragging)
{
	const float ratio = SliderRatio(value, minValue, maxValue);
	const bool hovered = CheckCollisionPointRec(GetMousePosition(),
		{track.x - 16.0f, track.y - 20.0f, track.width + 32.0f, track.height + 40.0f});

	DrawRectangleRounded(track, 0.5f, 16, theme::PanelBorder);

	Rectangle filled{track.x, track.y, track.width * ratio, track.height};
	if (filled.width > 1.0f)
		DrawRectangleRounded(filled, 0.5f, 16, theme::Accent);

	if (maxValue > minValue)
	{
		for (int tick = minValue; tick <= maxValue; tick += 5)
		{
			const float x = track.x + track.width * SliderRatio(tick, minValue, maxValue);
			const bool major = (tick == minValue || tick == maxValue);
			const float extent = major ? 9.0f : 5.0f;
			DrawLineEx({x, track.y - extent}, {x, track.y - 3.0f}, 1.5f, Fade(WHITE, major ? 0.32f : 0.16f));
			DrawLineEx({x, track.y + track.height + 3.0f}, {x, track.y + track.height + extent},
				1.5f, Fade(WHITE, major ? 0.32f : 0.16f));
		}
	}

	Vector2 thumb{track.x + track.width * ratio, track.y + track.height * 0.5f};
	if (dragging || hovered)
		DrawCircleV(thumb, 15.0f, Fade(theme::Accent, dragging ? 0.35f : 0.20f));

	DrawCircleV({thumb.x, thumb.y + 1.5f}, 10.5f, Fade(BLACK, 0.3f));
	DrawCircleV(thumb, 10.0f, theme::Text);
	DrawCircleV(thumb, 4.0f, theme::Accent);
}
} // namespace ui
