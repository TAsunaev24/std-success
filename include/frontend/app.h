#ifndef APP_H_
#define APP_H_

#include <array>

#include "ui.h"
#include "renderer.h"

class App
{
public:
	App();
	~App();
	int run();

private:
	enum class Screen
	{
		Menu,
		SizeSelect,
		Settings,
		Credits,
		Labyrinth
	};

	void update();
	void draw();

	void layoutMenu();
	void layoutSizeSelect();
	void layoutSettings();
	void layoutCredits();

	void updateMenu();
	void updateSizeSelect();
	void updateSettings();
	void updateCredits();
	void updateLabyrinth();

	void drawMenu();
	void drawSizeSelect();
	void drawSettings();
	void drawCredits();
	void drawLabyrinth();
	void drawFps();

	void activateMenu(int index);
	void toggleFullscreen();

	Renderer *_labyrinthRenderer = nullptr;

	Screen _screen = Screen::Menu;
	bool _closeRequested = false;

	bool _showFps = false;

	std::array<ui::Button, 4> _menuButtons;
	int _menuFocus = 0;

	int _mazeSize = 20;
	int _fogPreset = 0;
	bool _sliderDragging = false;
	Rectangle _sliderTrack{0, 0, 0, 0};
	std::array<ui::Button, 5> _presetButtons;
	std::array<ui::Button, 4> _fogButtons;
	ui::Button _sizeMinus{"-"};
	ui::Button _sizePlus{"+"};
	ui::Button _sizeStart{"START GAME", true};
	ui::Button _sizeBack{"BACK"};

	std::array<Rectangle, 2> _settingsRows{};
	ui::ToggleSwitch _fpsToggle;
	ui::ToggleSwitch _fullscreenToggle;
	ui::Button _settingsBack{"BACK"};

	Rectangle _creditsPanel{0, 0, 0, 0};
	ui::Button _creditsBack{"BACK"};
};

#endif
