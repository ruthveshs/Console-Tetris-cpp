# Console Tetris (C++)

A classic Tetris game built from scratch in C++ — no game engine, just core data structures and real-time console rendering.

## Features
- All 7 tetrominoes (I, O, T, S, Z, J, L) with rotation
- Collision detection and line clearing
- Real-time keyboard input (non-blocking, via `conio.h`)
- Gravity-based auto-drop on a timer
- Score and lines-cleared tracking

## How to Run
```bash
g++ tetris.cpp -o tetris
./tetris
```
**Windows only** (uses `conio.h` and `windows.h` for input/console handling).

## Controls
| Key | Action |
|-----|--------|
| A / D | Move left / right |
| S | Soft drop |
| W | Rotate |
| Space | Hard drop |
| Q | Quit |

## Core Concepts Used
- **2D arrays** for the board grid and piece shapes
- **Matrix rotation** for piece rotation logic
- **Reference parameters** for mutating piece position across function calls
- Collision detection via bounds + occupancy checks on every move

## What's Next
- Wall-kick handling on rotation near edges
- "Next piece" preview queue
- SFML graphics version

## Gameplay
<div align="center">
  <img width="480" height="435" alt="tetris_gameplay" src="https://github.com/user-attachments/assets/37cfe87e-648e-42f5-9218-af463cb042cf" />
</div>
