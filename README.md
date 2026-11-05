# std-success

A maze generator with a playable labyrinth game built with raylib.

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

## Game

- `Play` opens the maze size selection (10 x 10 to 50 x 50).
- `Settings` lets you toggle the FPS counter, timer and fullscreen mode.
- `Credits` shows project and controls information.
- `Quit` closes the game.

In game:

- `WASD` or arrow keys - move
- `R` - generate a new maze
- `Esc` - return to the menu
- `F11` - toggle fullscreen

The existing generator benchmarks can be run with `./build/pathtest <size> <count>`.
