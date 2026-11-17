# Board Game in C

🇫🇷 [Version française](README.fr.md)

A two-player strategy board game played in the terminal, written in C during the first year of a BUT Informatique (Bachelor of Technology in Computer Science) at the IUT Grand Ouest Normandie, Caen.

## The game

Two players, **North** and **South**, face each other on a 6×6 board with a goal on each side. The first player to bring a piece into the opponent's goal wins.

- 12 shared pieces: four of each of three sizes (1, 2 and 3). Pieces belong to neither player.
- **Placement phase:** each player places six pieces on the row closest to their side.
- **Movement phase:** on their turn, a player picks a piece from their own row and moves it a number of steps equal to its size, in the four directions. Only the last step may end on an occupied square, in which case the piece bounces off.

The complete rules are documented in the header `board.h`.

## What I wrote

| File | Content | Author |
|---|---|---|
| `board.c` | Implementation of the game engine: board state, placement, picking and moving pieces, bounces, victory detection | Mathéo Rimbault |
| `sae1-2.c` | Terminal game: board display, input handling, turn loop, end of game | Mathéo Rimbault |
| `board.h` | Interface and documentation of the game engine, **provided by the teaching team** (Paul Dorbec) | P. Dorbec |

## Build and run

Requirements: `gcc` and `make` (Linux, macOS, or WSL on Windows).

```bash
make
./jeu
```

Clean the build files with `make clean`.

## How to play

At each turn the program displays the board and asks for your moves (piece size, column, direction). Follow the prompts in the terminal.

## Concepts used

- Structures and opaque types (the `board` type is defined in `board.c`)
- Dynamic memory allocation and freeing (`new_game` / `destroy_game`)
- Modular programming with header files
- Input validation and game loop
- Compilation with `make`
