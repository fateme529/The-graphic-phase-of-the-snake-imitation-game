# The graphic phase of the snake imitation game

![Language](https://img.shields.io/badge/language-C%2B%2B-blue)
![Graphics](https://img.shields.io/badge/graphics-SFML-8CC445)
![Platform](https://img.shields.io/badge/platform-Windows-0078D6)
![License](https://img.shields.io/badge/license-MIT-green)

A graphical (SFML-based) follow-up to [Imitation of a Snake](https://github.com/fateme529/Imitation-of-a-snake),
a memory/maze puzzle inspired by the classic Snake game. The core game rules are the
same as the console version — only the rendering moved from a text console to a real
window with tiled graphics.

## How it works

The game builds a rectangular plane made of horizontal walls, each wall punctured with a
few randomly placed gaps ("holes"). It then computes a random valid path through one hole
in every wall, from the top of the plane to the bottom, and briefly animates a snake
moving through that correct path in the game window so you can memorize it.

Once the preview ends and "Your turn!" is shown, guide the snake down through the exact
same holes, row by row, using the keys below.

- **You win** when the snake reaches the final hole in the correct sequence.
- **You lose** if the snake hits a wall, or enters a hole that isn't the one from the
  memorized path.

A recorded demo is included:
[`Test.mp4`](<The graphic phase of the snake imitation game/Test.mp4>)
(it's a 2 MB video, too large for GitHub to preview inline — click **Raw** on that page
or clone the repo to watch it).

## Controls

| Key | Move  |
|-----|-------|
| W   | Up    |
| A   | Left  |
| S   | Down  |
| D   | Right |

## Project structure

```
The graphic phase of the snake imitation game/
  game.cpp                  # entry point: game loop, input handling, win/lose logic
  GameObjects.h              # Plane, Obstacle, and Snake classes (same logic as the console version)
  Graphic.h                  # SFML-based Display: renders the plane/snake as tiles in a window
  LiberationMono-Bold.ttf    # font used for the win/lose messages
  Test.mp4                   # recorded gameplay demo
```

## Dependencies

- [SFML](https://www.sfml-dev.org/) (`graphics`, `window`, and `system` modules) —
  required by `Graphic.h` for the window, drawing, and text.

## Building and running

This is a Windows console/GUI application built on SFML. There is no project file
checked in, so build it directly from the sources:

- **Visual Studio** (recommended): create a new C++ console project, add the source
  files, then [link SFML](https://www.sfml-dev.org/tutorials/) (e.g. via vcpkg:
  `vcpkg install sfml`) to the project — the `graphics`, `window`, and `system` modules.
- **g++ / MinGW**, once SFML is installed:
  ```bash
  g++ game.cpp -o snake.exe -lsfml-graphics -lsfml-window -lsfml-system
  ```

Run the resulting executable from inside the
`The graphic phase of the snake imitation game/` folder (or copy
`LiberationMono-Bold.ttf` next to the executable) — the win/lose message loads the font
from a path relative to the current working directory.
