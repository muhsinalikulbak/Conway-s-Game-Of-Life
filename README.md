# Life: Conway's Game of Life in the Terminal

A Game of Life simulator written in C. You draw the starting pattern with a tiny "pen" language piped into stdin, and the program animates the simulation in your terminal using **ncurses**, with colors that show how long each cell has been alive.

Based on the 42 `life` exam assignment, extended with a live visualizer.

## Features

- Initial board drawn with pen commands read from stdin
- Classic Conway rules, all cells updated simultaneously, cells outside the board are always dead
- Live ncurses animation with a blue-to-red age gradient (256-color terminals)
- Runtime controls: speed up, slow down, toggle gradient, quit
- Live counters for iteration number and alive cells

## Requirements

- A C compiler (`gcc` or `clang`) and `make`
- The ncurses development library
- A terminal with color support (256 colors for the gradient)

### Installing ncurses

| System | Command |
|---|---|
| Debian / Ubuntu / Mint | `sudo apt update && sudo apt install build-essential libncurses-dev` |
| Fedora / RHEL / CentOS | `sudo dnf install gcc make ncurses-devel` |
| Arch / Manjaro | `sudo pacman -S base-devel ncurses` |
| openSUSE | `sudo zypper install gcc make ncurses-devel` |
| macOS (Homebrew) | `xcode-select --install` then `brew install ncurses` |
| Windows | Use WSL (Ubuntu) and follow the Debian / Ubuntu line |

macOS already ships with a system ncurses, so after installing the Xcode command line tools the build usually works without Homebrew.

## Build

```bash
make
```

This produces the `./life` executable. To compile by hand instead:

```bash
gcc -Wall -Wextra -Werror src/*.c -o life -lncurses
```

## Usage

```bash
./life <width> <height> <iterations>
```

| Argument | Meaning |
|---|---|
| `width` | Board width (columns), must be > 0 |
| `height` | Board height (rows), must be > 0 |
| `iterations` | Number of generations to run, 0 or more |

The starting pattern is read from **stdin** until EOF, so always pipe or redirect it in:

```bash
echo "dxdxsaxaxdsxx" | ./life 100 50 1000
./life 100 50 1000 < pattern.txt
```

If you run `./life 100 50 1000` with nothing piped in, the program waits for input. Type your commands and press `Ctrl+D`.

### Terminal size

The board is drawn one row below the top (row 0 is the status line), and the welcome message starts at column 70. For `width x height` use a terminal of at least **`width` columns and `height + 1` rows** (and about 100 columns for the welcome message). If the board is larger than your terminal, the part outside the screen is not visible.

## Drawing language

A pen starts in the top-left corner of the board (row 0, column 0), **lifted** (not drawing).

| Command | Action |
|---|---|
| `w` | Move pen up |
| `a` | Move pen left |
| `s` | Move pen down |
| `d` | Move pen right |
| `x` | Toggle pen up / down |

Rules:

- While the pen is down, every cell it stands on becomes alive, including the cell where it was lowered.
- The pen **cannot leave the board**. A move that would cross the edge is ignored and the pen stays on the border.
- Newlines are ignored. Any other character is an error and the program exits with status 1.

Example: `sdxddssaaww` on a 5x5 board draws a square ring.

```
$ echo 'sdxddssaaww' | ./life 5 5 0
     
 OOO 
 O O 
 OOO 
     
```

### Tip: start from the middle

Pen movement with the pen up draws nothing, so you can travel to the center first. In bash, `printf 'd%.0s' {1..50}` prints fifty `d` characters:

```bash
{ printf 'd%.0s' {1..50}; printf 's%.0s' {1..25}; echo "PATTERN"; } | ./life 100 50 1200
```

This moves the pen to column 50, row 25 (the middle of a 100x50 board) before drawing `PATTERN`. Starting a pattern in a corner often makes it die quickly, because it cannot grow beyond the walls.

## In-game controls

| Key | Action |
|---|---|
| `q` / `Q` | Quit |
| `+` | Faster (shorter delay) |
| `-` | Slower (longer delay) |
| `Space` | Toggle the age gradient on / off |

The status line shows the current iteration and the number of alive cells.

## Suggested patterns to try

All commands assume a terminal of at least 100 columns by 51 rows.

**R-pentomino** from the center: 5 cells, long chaotic growth.

```bash
{ printf 'd%.0s' {1..50}; printf 's%.0s' {1..25}; echo dxdxsaxaxdsxx; } | ./life 100 50 1200
```

The same pattern in the corner, to see the wall effect (it dies out within a few generations):

```bash
echo "dxdxsaxaxdsxx" | ./life 100 50 1200
```

The same R-pentomino with the movement typed out by hand:

```bash
echo "ddddddddddddddddddddddddddddddddddddddddddddddddddsssssssssssssssssssssssssdxdxsaxaxdsxx" | ./life 100 50 1200
```

**Acorn** from the center: 7 cells, a long-lived and chaotic pattern (the walls change how it ends compared to an infinite board).

```bash
{ printf 'd%.0s' {1..45}; printf 's%.0s' {1..25}; echo dxxsddxxsaaaxdxdddxddx; } | ./life 100 50 3000
```

**Glider** from the top-left corner: travels diagonally and crashes into the bottom-right wall.

```bash
echo "dxxsdxxsxaax" | ./life 100 50 400
```

**Blinker** (period 2 oscillator), a quick sanity check:

```bash
echo "dddddxss" | ./life 11 5 100
```

**Block** (still life), it never changes:

```bash
echo "sdxdsaxa" | ./life 10 6 100
```

**Exam-style check** with no animation worth watching, just a small board:

```bash
echo "dxss" | ./life 3 3 1
```

## Project layout

```
.
├── Makefile
├── README.md
└── src/
    ├── main.c
    ├── init_board.c / init_board.h
    ├── game_loop.c / game_loop.h
    └── utils.c / utils.h
```

| File | Role |
|---|---|
| `src/main.c` | Argument parsing, board allocation, program flow |
| `src/init_board.c/.h` | Reads stdin and draws the starting board from pen commands |
| `src/game_loop.c/.h` | Conway rules, ncurses setup, rendering, key handling |
| `src/utils.c/.h` | Small helpers (`ft_strlen`, `ft_memcpy`, `free_all`) |

## How it works

1. Stdin is read fully, then the pen commands are applied to a `height x width` board.
2. Stdin is re-attached to `/dev/tty` so ncurses can still read the keyboard even though the pattern came through a pipe.
3. Each generation is computed with temporary markers (`'2'` = dies, `'/'` = born) so all cells update simultaneously, then the markers are resolved.
4. Each cell's age is tracked and mapped to a color from a 256-color gradient.

## Troubleshooting

- **Screen goes black right away:** the pattern probably died out. Start it away from the walls (see "start from the middle").
- **Nothing happens until I press Ctrl+D:** you did not pipe any input. Use `echo "..." | ./life ...`.
- **No colors / program exits immediately:** the terminal reports no color support. Try `export TERM=xterm-256color`.
- **`ncurses.h: No such file or directory`:** the development package is missing, see the install table above.
- **Linker error `undefined reference to initscr`:** add `-lncurses` to the link step.