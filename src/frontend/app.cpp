#include "app.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>

namespace
{
constexpr int MinMazeSize = 10;
constexpr int MaxMazeSize = 50;

bool IsEnterPressed()
{
	return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);
}

std::string FormatTime(float seconds)
{
	if (seconds < 0.0f) seconds = 0.0f;
	const int tenths = static_cast<int>(seconds * 10.0f + 0.5f);
	const int minutes = tenths / 600;
	const int wholeSeconds = (tenths / 10) % 60;
	const int decimal = tenths % 10;

	char buffer[32];
	std::snprintf(buffer, sizeof(buffer), "%02d:%02d.%d", minutes, wholeSeconds, decimal);
	return buffer;
}

void SetPointerCursor(bool pointing)
{
	SetMouseCursor(pointing ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);
}
} // namespace

App::App()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
	InitWindow(1280, 800, "Labyrinth");
	SetWindowMinSize(900, 600);
	SetExitKey(KEY_NULL);
	SetTargetFPS(60);

	_menuButtons[0] = ui::Button("PLAY", true);
	_menuButtons[1] = ui::Button("SETTINGS");
	_menuButtons[2] = ui::Button("CREDITS");
	_menuButtons[3] = ui::Button("QUIT");

	for (int i = 0; i < 5; ++i)
	{
		const int size = (i + 1) * 10;
		_presetButtons[i] = ui::Button(std::to_string(size) + " x " + std::to_string(size));
	}
}

App::~App()
{
	if (IsWindowReady())
		CloseWindow();
}

int App::run()
{
	while (!_closeRequested && !WindowShouldClose())
	{
		const float dt = GetFrameTime();
		update(dt);
		draw();
	}
	return 0;
}

void App::update(float dt)
{
	if (IsKeyPressed(KEY_F11))
		toggleFullscreen();

	switch (_screen)
	{
		case Screen::Menu:
			updateMenu();
			break;
		case Screen::SizeSelect:
			updateSizeSelect();
			break;
		case Screen::Settings:
			updateSettings();
			break;
		case Screen::Credits:
			updateCredits();
			break;
		case Screen::Game:
			updateGame(dt);
			break;
	}
}

void App::draw()
{
	BeginDrawing();
	ClearBackground(theme::BackgroundBottom);

	switch (_screen)
	{
		case Screen::Menu:
			drawMenu();
			break;
		case Screen::SizeSelect:
			drawSizeSelect();
			break;
		case Screen::Settings:
			drawSettings();
			break;
		case Screen::Credits:
			drawCredits();
			break;
		case Screen::Game:
			drawGame();
			break;
	}

	drawFps();
	EndDrawing();
}

void App::layoutMenu()
{
	const int width = GetScreenWidth();
	const int height = GetScreenHeight();

	const float buttonWidth = std::clamp(width * 0.34f, 300.0f, 440.0f);
	const float buttonHeight = std::clamp(height * 0.077f, 54.0f, 68.0f);
	const float gap = std::clamp(height * 0.021f, 13.0f, 20.0f);
	const float totalHeight = buttonHeight * 4.0f + gap * 3.0f;

	float startY = height * 0.40f;
	if (startY + totalHeight > height - 80.0f)
		startY = height - 80.0f - totalHeight;
	if (startY < height * 0.32f)
		startY = height * 0.32f;

	const float x = (width - buttonWidth) * 0.5f;
	for (int i = 0; i < 4; ++i)
		_menuButtons[i].rect = {x, startY + i * (buttonHeight + gap), buttonWidth, buttonHeight};
}

void App::layoutSizeSelect()
{
	const int width = GetScreenWidth();
	const int height = GetScreenHeight();
	const float centerX = width * 0.5f;

	_sizeBack.rect = {32.0f, 28.0f, 120.0f, 44.0f};

	const float sliderWidth = std::clamp(width * 0.52f, 360.0f, 620.0f);
	const float sliderCenterY = height * 0.46f;
	_sliderTrack = {centerX - sliderWidth * 0.5f, sliderCenterY - 4.0f, sliderWidth, 8.0f};

	_sizeMinus.rect = {_sliderTrack.x - 64.0f, sliderCenterY - 24.0f, 48.0f, 48.0f};
	_sizePlus.rect = {_sliderTrack.x + sliderWidth + 16.0f, sliderCenterY - 24.0f, 48.0f, 48.0f};

	const float presetWidth = std::clamp((width - 80.0f - 4.0f * 12.0f) / 5.0f, 64.0f, 96.0f);
	constexpr float presetHeight = 42.0f;
	constexpr float presetGap = 12.0f;
	const float presetsWidth = presetWidth * 5.0f + presetGap * 4.0f;
	const float presetsY = height * 0.565f;

	for (int i = 0; i < 5; ++i)
		_presetButtons[i].rect = {centerX - presetsWidth * 0.5f + i * (presetWidth + presetGap),
			presetsY, presetWidth, presetHeight};

	_sizeStart.rect = {centerX - 160.0f, height * 0.72f, 320.0f, 60.0f};
}

void App::layoutSettings()
{
	const int width = GetScreenWidth();
	const int height = GetScreenHeight();

	const float rowWidth = std::clamp(width * 0.62f, 480.0f, 700.0f);
	constexpr float rowHeight = 68.0f;
	constexpr float rowGap = 14.0f;
	const float startY = height * 0.30f;
	const float x = (width - rowWidth) * 0.5f;

	for (int i = 0; i < 3; ++i)
		_settingsRows[i] = {x, startY + i * (rowHeight + rowGap), rowWidth, rowHeight};

	const Rectangle &first = _settingsRows[0];
	const Rectangle &second = _settingsRows[1];
	const Rectangle &third = _settingsRows[2];
	const float toggleOffsetX = rowWidth - 24.0f - 64.0f;
	_fpsToggle.rect = {first.x + toggleOffsetX, first.y + (rowHeight - 32.0f) * 0.5f, 64.0f, 32.0f};
	_timerToggle.rect = {second.x + toggleOffsetX, second.y + (rowHeight - 32.0f) * 0.5f, 64.0f, 32.0f};
	_fullscreenToggle.rect = {third.x + toggleOffsetX, third.y + (rowHeight - 32.0f) * 0.5f, 64.0f, 32.0f};

	_settingsBack.rect = {width * 0.5f - 110.0f, startY + 3.0f * rowHeight + 2.0f * rowGap + 40.0f,
		220.0f, 54.0f};
}

void App::layoutCredits()
{
	const int width = GetScreenWidth();
	const int height = GetScreenHeight();

	const float panelWidth = std::clamp(width * 0.66f, 520.0f, 720.0f);
	const float panelHeight = std::clamp(height * 0.68f, 390.0f, 520.0f);
	_creditsPanel = {(width - panelWidth) * 0.5f, height * 0.18f, panelWidth, panelHeight};
	_creditsBack.rect = {width * 0.5f - 110.0f, _creditsPanel.y + _creditsPanel.height - 74.0f,
		220.0f, 54.0f};
}

void App::layoutGame()
{
	const int width = GetScreenWidth();
	const int height = GetScreenHeight();

	_gameMenu.rect = {24.0f, 18.0f, 104.0f, 40.0f};

	const float panelWidth = std::clamp(width * 0.52f, 440.0f, 580.0f);
	constexpr float panelHeight = 330.0f;
	_winPanel = {(width - panelWidth) * 0.5f, (height - panelHeight) * 0.5f, panelWidth, panelHeight};

	const float buttonWidth = (panelWidth - 72.0f) * 0.5f;
	const float buttonY = _winPanel.y + panelHeight - 88.0f;
	_winAgain.rect = {_winPanel.x + 24.0f, buttonY, buttonWidth, 56.0f};
	_winMenu.rect = {_winPanel.x + panelWidth - 24.0f - buttonWidth, buttonY, buttonWidth, 56.0f};
}

void App::activateMenu(int index)
{
	switch (index)
	{
		case 0:
			_screen = Screen::SizeSelect;
			break;
		case 1:
			_screen = Screen::Settings;
			break;
		case 2:
			_screen = Screen::Credits;
			break;
		case 3:
			_closeRequested = true;
			break;
	}
}

void App::startGame()
{
	_game = std::make_unique<MazeGame>(_mazeSize);
	_screen = Screen::Game;
}

void App::goToMenu()
{
	_game.reset();
	_screen = Screen::Menu;
}

void App::toggleFullscreen()
{
	ToggleFullscreen();
}

void App::updateMenu()
{
	layoutMenu();
	const Vector2 mouse = GetMousePosition();
	bool handCursor = false;
	int clicked = -1;

	for (int i = 0; i < 4; ++i)
	{
		_menuButtons[i].update(mouse);
		if (_menuButtons[i].hovered)
		{
			_menuFocus = i;
			handCursor = true;
		}
		if (_menuButtons[i].clicked())
			clicked = i;
	}

	if (clicked >= 0)
	{
		SetPointerCursor(handCursor);
		activateMenu(clicked);
		return;
	}

	if (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN))
		_menuFocus = (_menuFocus + 1) % 4;
	if (IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP))
		_menuFocus = (_menuFocus + 3) % 4;

	for (int i = 0; i < 4; ++i)
		_menuButtons[i].focused = (_menuFocus == i);

	if (IsEnterPressed())
	{
		SetPointerCursor(handCursor);
		activateMenu(_menuFocus);
		return;
	}

	if (IsKeyPressed(KEY_ESCAPE))
		_closeRequested = true;

	SetPointerCursor(handCursor);
}

void App::updateSizeSelect()
{
	layoutSizeSelect();
	const Vector2 mouse = GetMousePosition();

	ui::UpdateSliderInt(_sliderTrack, _mazeSize, MinMazeSize, MaxMazeSize, _sliderDragging);

	_sizeMinus.update(mouse);
	_sizePlus.update(mouse);
	_sizeStart.update(mouse);
	_sizeBack.update(mouse);
	for (ui::Button &button : _presetButtons)
		button.update(mouse);

	bool handCursor = _sizeMinus.hovered || _sizePlus.hovered || _sizeStart.hovered ||
		_sizeBack.hovered || _sliderDragging ||
		CheckCollisionPointRec(mouse, {_sliderTrack.x - 16.0f, _sliderTrack.y - 20.0f,
			_sliderTrack.width + 32.0f, _sliderTrack.height + 40.0f});
	for (const ui::Button &button : _presetButtons)
		handCursor = handCursor || button.hovered;

	if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT))
		_mazeSize = std::max(MinMazeSize, _mazeSize - 1);
	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT))
		_mazeSize = std::min(MaxMazeSize, _mazeSize + 1);

	if (_sizeMinus.clicked())
		_mazeSize = std::max(MinMazeSize, _mazeSize - 1);
	if (_sizePlus.clicked())
		_mazeSize = std::min(MaxMazeSize, _mazeSize + 1);

	for (int i = 0; i < 5; ++i)
	{
		if (_presetButtons[i].clicked())
			_mazeSize = (i + 1) * 10;
	}

	if (_sizeStart.clicked() || IsEnterPressed())
	{
		SetPointerCursor(handCursor);
		startGame();
		return;
	}

	if (_sizeBack.clicked() || IsKeyPressed(KEY_ESCAPE))
	{
		SetPointerCursor(handCursor);
		_screen = Screen::Menu;
		return;
	}

	SetPointerCursor(handCursor);
}

void App::updateSettings()
{
	layoutSettings();
	const Vector2 mouse = GetMousePosition();

	_fpsToggle.update(mouse);
	_timerToggle.update(mouse);
	_fullscreenToggle.update(mouse);
	_settingsBack.update(mouse);

	const bool handCursor = _fpsToggle.hovered || _timerToggle.hovered ||
		_fullscreenToggle.hovered || _settingsBack.hovered;

	if (_fpsToggle.clicked())
		_showFps = !_showFps;
	if (_timerToggle.clicked())
		_showTimer = !_showTimer;
	if (_fullscreenToggle.clicked())
		toggleFullscreen();

	if (_settingsBack.clicked() || IsKeyPressed(KEY_ESCAPE))
	{
		SetPointerCursor(handCursor);
		_screen = Screen::Menu;
		return;
	}

	SetPointerCursor(handCursor);
}

void App::updateCredits()
{
	layoutCredits();
	const Vector2 mouse = GetMousePosition();
	_creditsBack.update(mouse);

	if (_creditsBack.clicked() || IsKeyPressed(KEY_ESCAPE) || IsEnterPressed())
	{
		SetPointerCursor(_creditsBack.hovered);
		_screen = Screen::Menu;
		return;
	}

	SetPointerCursor(_creditsBack.hovered);
}

void App::updateGame(float dt)
{
	layoutGame();
	const Vector2 mouse = GetMousePosition();

	if (!_game)
	{
		_screen = Screen::Menu;
		return;
	}

	_gameMenu.update(mouse);
	bool handCursor = _gameMenu.hovered;

	if (_game->won())
	{
		_winAgain.update(mouse);
		_winMenu.update(mouse);
		handCursor = handCursor || _winAgain.hovered || _winMenu.hovered;
	}

	if (_gameMenu.clicked())
	{
		SetPointerCursor(handCursor);
		goToMenu();
		return;
	}

	if (IsKeyPressed(KEY_ESCAPE))
	{
		SetPointerCursor(handCursor);
		goToMenu();
		return;
	}

	if (_game->won())
	{
		if (_winAgain.clicked() || IsEnterPressed())
		{
			_game->restart();
			SetPointerCursor(handCursor);
			return;
		}
		else if (_winMenu.clicked())
		{
			SetPointerCursor(handCursor);
			goToMenu();
			return;
		}
	}

	if (IsKeyPressed(KEY_R))
	{
		_game->restart();
		SetPointerCursor(handCursor);
		return;
	}

	_game->update(dt);
	SetPointerCursor(handCursor);
}

void App::drawFps()
{
	if (!_showFps) return;

	char buffer[32];
	std::snprintf(buffer, sizeof(buffer), "FPS %d", GetFPS());
	ui::DrawTextRight(buffer, static_cast<float>(GetScreenWidth()) - 24.0f,
		static_cast<float>(GetScreenHeight()) - 24.0f, 15.0f, theme::TextMuted, 1.0f);
}

void App::drawMenu()
{
	layoutMenu();
	ui::DrawBackground();

	const int width = GetScreenWidth();
	const int height = GetScreenHeight();
	const float titleSize = std::clamp(height * 0.105f, 60.0f, 88.0f);
	const float titleY = height * 0.135f;

	ui::DrawTextCentered("LABYRINTH", titleY + 3.0f, titleSize, Fade(theme::Accent, 0.55f), 4.0f);
	ui::DrawTextCentered("LABYRINTH", titleY, titleSize, theme::Text, 4.0f);

	const float lineY = titleY + titleSize + 22.0f;
	DrawRectangleRounded({width * 0.5f - 60.0f, lineY, 120.0f, 4.0f}, 0.5f, 8, theme::Accent);
	ui::DrawTextCentered("FIND YOUR WAY OUT", lineY + 20.0f, 15.0f, theme::TextMuted, 4.0f);

	for (const ui::Button &button : _menuButtons)
		button.draw();

	ui::DrawTextCentered("MOUSE OR UP/DOWN + ENTER", height - 54.0f, 15.0f,
		Fade(theme::TextMuted, 0.9f), 2.0f);
}

void App::drawSizeSelect()
{
	layoutSizeSelect();
	ui::DrawBackground();

	const int height = GetScreenHeight();

	_sizeBack.draw();

	ui::DrawTextCentered("MAZE SIZE", height * 0.075f, 44.0f, theme::Text, 3.0f);
	ui::DrawTextCentered("CHOOSE FROM 10 X 10 UP TO 50 X 50", height * 0.075f + 58.0f, 16.0f,
		theme::TextMuted, 2.0f);

	ui::DrawTextCentered("CURRENT SIZE", height * 0.205f, 13.0f, theme::TextMuted, 4.0f);
	const std::string sizeLabel = std::to_string(_mazeSize) + " x " + std::to_string(_mazeSize);
	const float sizeFont = std::clamp(height * 0.115f, 64.0f, 96.0f);
	ui::DrawTextCentered(sizeLabel.c_str(), height * 0.23f, sizeFont, theme::Accent, 2.0f);

	ui::DrawSliderInt(_sliderTrack, _mazeSize, MinMazeSize, MaxMazeSize, _sliderDragging);
	ui::DrawTextCenteredX("10", _sliderTrack.x, _sliderTrack.y + 15.0f, 14.0f, theme::TextMuted, 1.0f);
	ui::DrawTextCenteredX("50", _sliderTrack.x + _sliderTrack.width, _sliderTrack.y + 15.0f,
		14.0f, theme::TextMuted, 1.0f);

	_sizeMinus.draw();
	_sizePlus.draw();

	ui::DrawTextCentered("QUICK SELECT", _presetButtons[0].rect.y - 24.0f, 12.0f,
		theme::TextMuted, 3.0f);
	for (const ui::Button &button : _presetButtons)
		button.draw();

	_sizeStart.draw();

	ui::DrawTextCentered("LEFT/RIGHT: CHANGE SIZE    ENTER: START    ESC: BACK",
		height - 36.0f, 14.0f, theme::TextMuted, 1.5f);
}

void App::drawSettings()
{
	layoutSettings();
	ui::DrawBackground();

	const int height = GetScreenHeight();
	const char *labels[3] = {"SHOW FPS COUNTER", "SHOW TIMER", "FULLSCREEN"};
	const bool values[3] = {_showFps, _showTimer, IsWindowFullscreen()};
	const ui::ToggleSwitch *toggles[3] = {&_fpsToggle, &_timerToggle, &_fullscreenToggle};

	ui::DrawTextCentered("SETTINGS", height * 0.10f, 48.0f, theme::Text, 3.0f);
	ui::DrawTextCentered("TUNE THE EXPERIENCE", height * 0.10f + 62.0f, 16.0f,
		theme::TextMuted, 3.0f);

	for (int i = 0; i < 3; ++i)
	{
		const Rectangle &row = _settingsRows[i];
		const bool hovered = toggles[i]->hovered;
		DrawRectangleRounded(row, 0.18f, 10, hovered ? theme::PanelHover : theme::Panel);
		DrawRectangleRoundedLinesEx(row, 0.18f, 10, hovered ? 1.5f : 1.0f,
			hovered ? theme::Accent : theme::PanelBorder);

		constexpr float labelSize = 20.0f;
		Vector2 measured = MeasureTextEx(GetFontDefault(), labels[i], labelSize, 2.0f);
		DrawTextEx(GetFontDefault(), labels[i],
			{row.x + 26.0f, row.y + (row.height - measured.y) * 0.5f},
			labelSize, 2.0f, theme::Text);

		toggles[i]->draw(values[i]);
	}

	_settingsBack.draw();

	ui::DrawTextCentered("F11 TOGGLES FULLSCREEN FROM ANY SCREEN", height - 36.0f, 14.0f,
		theme::TextMuted, 1.5f);
}

void App::drawCredits()
{
	layoutCredits();
	ui::DrawBackground();

	const int height = GetScreenHeight();
	const Rectangle &panel = _creditsPanel;
	const float left = panel.x + 42.0f;

	ui::DrawTextCentered("CREDITS", height * 0.08f, 48.0f, theme::Text, 3.0f);
	ui::DrawTextCentered("THANKS FOR PLAYING", height * 0.08f + 62.0f, 16.0f,
		theme::TextMuted, 3.0f);

	DrawRectangleRounded({panel.x, panel.y + 5.0f, panel.width, panel.height}, 0.05f, 12,
		Fade(BLACK, 0.28f));
	DrawRectangleRounded(panel, 0.05f, 12, Fade(theme::Panel, 0.96f));
	DrawRectangleRoundedLinesEx(panel, 0.05f, 12, 1.5f, theme::PanelBorder);

	float y = panel.y + 30.0f;
	DrawTextEx(GetFontDefault(), "BUILT WITH", {left, y}, 12.0f, 3.0f, theme::Accent);
	y += 24.0f;
	DrawTextEx(GetFontDefault(), "C++ and raylib", {left, y}, 20.0f, 1.5f, theme::Text);
	y += 34.0f;
	DrawTextEx(GetFontDefault(), "MAZE GENERATION", {left, y}, 12.0f, 3.0f, theme::Accent);
	y += 24.0f;
	DrawTextEx(GetFontDefault(), "labgen (std-success)", {left, y}, 20.0f, 1.5f, theme::Text);
	y += 34.0f;
	DrawTextEx(GetFontDefault(), "CONTROLS", {left, y}, 12.0f, 3.0f, theme::Accent);
	y += 24.0f;
	DrawTextEx(GetFontDefault(), "WASD / arrow keys - move", {left, y}, 19.0f, 1.5f, theme::Text);
	y += 26.0f;
	DrawTextEx(GetFontDefault(), "R - new maze, Esc - back to menu", {left, y}, 19.0f, 1.5f,
		theme::Text);
	y += 36.0f;
	DrawTextEx(GetFontDefault(), "SOURCE", {left, y}, 12.0f, 3.0f, theme::Accent);
	y += 24.0f;
	DrawTextEx(GetFontDefault(), "github.com/TAsunaev24/std-success", {left, y}, 17.0f, 1.5f,
		theme::TextMuted);

	_creditsBack.draw();

	ui::DrawTextCentered("ESC OR ENTER TO RETURN", height - 36.0f, 14.0f, theme::TextMuted, 1.5f);
}

void App::drawGame()
{
	layoutGame();

	if (!_game)
		return;

	_game->draw();

	const int width = GetScreenWidth();
	const int height = GetScreenHeight();
	const int hudHeight = static_cast<int>(MazeGame::HUD_HEIGHT);
	const int footerHeight = static_cast<int>(MazeGame::FOOTER_HEIGHT);

	DrawRectangle(0, 0, width, hudHeight, Fade(theme::Panel, 0.96f));
	DrawRectangle(0, hudHeight - 1, width, 1, theme::PanelBorder);

	_gameMenu.draw();

	const std::string title = "MAZE " + std::to_string(_game->size()) + " x " +
		std::to_string(_game->size());
	ui::DrawTextCenteredX(title.c_str(), width * 0.5f, 20.0f, 26.0f, theme::Text, 2.0f);
	ui::DrawTextCenteredX("FIND THE GOLDEN EXIT", width * 0.5f, 50.0f, 13.0f,
		theme::TextMuted, 2.0f);

	const float right = width - 24.0f;
	const std::string moves = "MOVES " + std::to_string(_game->moves());
	if (_showTimer)
	{
		const std::string time = "TIME " + FormatTime(_game->elapsed());
		ui::DrawTextRight(time.c_str(), right, 18.0f, 16.0f, theme::Text, 1.0f);
		ui::DrawTextRight(moves.c_str(), right, 44.0f, 16.0f, theme::TextMuted, 1.0f);
	}
	else
	{
		ui::DrawTextRight(moves.c_str(), right, 31.0f, 16.0f, theme::Text, 1.0f);
	}

	DrawRectangle(0, height - footerHeight, width, footerHeight, Fade(theme::Panel, 0.82f));
	DrawRectangle(0, height - footerHeight, width, 1, theme::PanelBorder);
	ui::DrawTextCentered("WASD / ARROWS: MOVE    R: NEW MAZE    ESC: MENU", height - 30.0f,
		15.0f, theme::TextMuted, 1.5f);

	if (_game->won())
	{
		DrawRectangle(0, 0, width, height, Fade(BLACK, 0.62f));

		const Rectangle &panel = _winPanel;
		const float centerX = panel.x + panel.width * 0.5f;

		DrawRectangleRounded({panel.x, panel.y + 6.0f, panel.width, panel.height}, 0.05f, 12,
			Fade(BLACK, 0.35f));
		DrawRectangleRounded(panel, 0.05f, 12, theme::Panel);
		DrawRectangleRoundedLinesEx(panel, 0.05f, 12, 2.0f, theme::Accent);

		ui::DrawTextCenteredX("MAZE SOLVED!", centerX, panel.y + 34.0f, 40.0f,
			theme::Success, 3.0f);
		ui::DrawTextCenteredX("YOU FOUND THE WAY OUT", centerX, panel.y + 88.0f, 14.0f,
			theme::TextMuted, 3.0f);

		const float statY = panel.y + 126.0f;
		const float statWidth = (panel.width - 80.0f) * 0.5f;
		Rectangle timeBox{panel.x + 32.0f, statY, statWidth, 72.0f};
		Rectangle movesBox{panel.x + panel.width - 32.0f - statWidth, statY, statWidth, 72.0f};

		DrawRectangleRounded(timeBox, 0.20f, 10, theme::PanelHover);
		DrawRectangleRounded(movesBox, 0.20f, 10, theme::PanelHover);

		ui::DrawTextCenteredX("TIME", timeBox.x + statWidth * 0.5f, statY + 11.0f, 12.0f,
			theme::TextMuted, 2.0f);
		ui::DrawTextCenteredX(FormatTime(_game->elapsed()).c_str(),
			timeBox.x + statWidth * 0.5f, statY + 34.0f, 26.0f, theme::Text, 1.5f);

		ui::DrawTextCenteredX("MOVES", movesBox.x + statWidth * 0.5f, statY + 11.0f, 12.0f,
			theme::TextMuted, 2.0f);
		ui::DrawTextCenteredX(std::to_string(_game->moves()).c_str(),
			movesBox.x + statWidth * 0.5f, statY + 34.0f, 26.0f, theme::Text, 1.5f);

		_winAgain.draw();
		_winMenu.draw();
	}
}
