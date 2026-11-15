# std-success

A labyrinth game with procedural maze generation and fog of war built with C++ and raylib.

## Build

Make sure the raylib submodule is initialized, then configure and build:

```sh
git submodule update --init --recursive
cmake -S . -B build
cmake --build build -j
```

Run the game:

```sh
./build/labyrinth
```

## Features & Menu

- `Play` opens the game setup screen:
  - Choose maze size from `10 x 10` to `50 x 50` using the slider, `-`/`+` buttons, or quick presets.
  - Select fog of war difficulty: `Easy` (`size / 4 * 3`), `Medium` (`size / 2`), `Hard` (`size / 4`), or `Very Hard` (`size / 6`).
- `Settings` lets you toggle the FPS counter and fullscreen mode.
- `Credits` shows project and controls information.
- `Quit` closes the app.

## Controls

- `Up` / `Down` / `Left` / `Right` — move through the labyrinth (or adjust size in the setup menu)
- `Enter` — confirm menu selection / start game
- `Esc` — return to the previous screen / quit from main menu
- `F11` — toggle fullscreen from any screen

## Testing & Benchmarks

Additional CLI utilities are built alongside the main executable:

- `./build/labtest <size>` — generates a maze of the given size and prints it to the terminal.
- `./build/pathtest <size> <count>` — benchmarks generation success rate and average wall ratio over `<count>` runs.
- `./build/tablegen` — runs unit tests for maze passability, wall congestion, and RNG distribution, exporting the report to `results.csv`.
