# std-success

A maze menu interface built with raylib.

## Build

Make sure the raylib submodule is initialized, then configure and build:

```sh
git submodule update --init --recursive
cmake -S . -B build
cmake --build build -j
```

Run the app:

```sh
./build/labyrinth
```

## Menu

- `Play` opens the maze size selection (10 x 10 to 50 x 50).
- `Settings` lets you toggle the FPS counter, timer and fullscreen mode.
- `Credits` shows project and controls information.
- `Quit` closes the app.
- `F11` toggles fullscreen from any screen.

The size selection dialog is kept as a placeholder for the future game screen;
the gameplay itself is not included in this version.

The existing generator benchmarks can be run with `./build/pathtest <size> <count>`.
