# 2048 Game

A terminal-based implementation of the classic **2048 puzzle game** written in **C**. Move the tiles using your keyboard and combine matching numbers to reach 2048!  
Created by **Satya Nihal Kodukula** as a practical application of low-level memory and pointer manipulation concepts learned in **CISC 221: Computer Architecture** and **CISC 220: System Level Programming**.  
Inspired by the [GeeksforGeeks 2048 game in C](https://www.geeksforgeeks.org/2048-game-in-c/).

## Features

- **Classic Gameplay**: Combine tiles of the same value by sliding in one of four directions to reach 2048.
- **Undo Moves**: Go back to a previous game state using the `P` key.
- **Persistent High Score**: Tracks and saves the highest score across sessions.
- **Clear Text UI**: Displays the game board, current score, and controls using simple terminal formatting.
- **Random Tile Insertion**: After each move, a new tile (2 or 4) is randomly placed on the grid.

## Controls

| Key | Action        |
|-----|---------------|
| W   | Move Up       |
| S   | Move Down     |
| A   | Move Left     |
| D   | Move Right    |
| P   | Undo Previous |
| R   | Restart Game  |
| U   | Exit Game     |

## How It Works

1. **Initialization**:
   - The game starts with a 4x4 grid initialized with one random tile.
   - A score counter starts at 0, and a `highscore.txt` file tracks the highest score achieved.

2. **Rendering**:
   - The terminal screen is cleared and redrawn each time the board is updated.
   - The board is printed with centered cells for readability.

3. **Game Logic**:
   - When a move is made, tiles slide in the given direction and combine if two adjacent tiles have the same value.
   - Only one combination per move direction is allowed for each tile row/column.

4. **Undo Feature**:
   - Pressing `P` restores the board and score to the previous move state using a 3D array-based history and a score log.

5. **Ending the Game**:
   - The game ends when there are no more legal moves and prompts the user to play again or quit.

## Requirements

- **GCC (C Compiler)** with support for `stdlib.h`, `time.h`, and basic file I/O.
- Works best on **Unix-like systems** (Linux, macOS). Minor modifications may be required for Windows (e.g., replacing `system("clear")`).

## Compilation

```bash
gcc -o game2048 game2048.c
