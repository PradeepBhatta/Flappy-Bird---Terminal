# Flappy Bird — Terminal Game in C

A simple flappy bird game that runs in your terminal. Made for our freshman C programming project.

---

## How to Run

**Linux / Mac:**
```
gcc flappy_bird.c -o flappy_bird
./flappy_bird
```

**Windows:**
```
gcc flappy_bird.c -o flappy_bird.exe
flappy_bird.exe
```

---

## How to Play

- The bird `@` falls down by itself
- Press **any key** to make it jump up
- Get through the gap in the wall `#` without hitting it
- Every wall you pass = **+1 point**
- Hit a wall or go off screen = **Game Over**
- Press **Q** anytime to quit

---

## Variables — What They Are and What They Do

| Variable | What it is | What it's used for |
|---|---|---|
| `bird_row` | A number (0 to 14) | Tracks where the bird is up and down on the screen |
| `wall_column` | A number (0 to 49) | Tracks how far left the wall has moved |
| `opening_start_row` | A number | Tells us where the gap in the wall starts |
| `player_score` | A number starting at 0 | Goes up by 1 every time you pass a wall |
| `is_game_over` | 0 or 1 | 0 means game is running, 1 means game has ended |
| `saved_terminal_settings` | Terminal config | Saves the original terminal so we can restore it after the game (Linux/Mac only) |
| `BOARD_WIDTH` | Set to 50 | How wide the game board is |
| `BOARD_HEIGHT` | Set to 15 | How tall the game board is |
| `WALL_OPENING_SIZE` | Set to 4 | How big the gap in the wall is |

---

## Functions — What Each One Does

### `fix_terminal_for_game()`
> *Linux/Mac only*

Normally the terminal waits for you to press Enter before reading any key. That's annoying in a game. This function turns that off so the game can read your key press instantly without waiting.

---

### `restore_terminal_to_normal()`
> *Linux/Mac only*

After the game is done, this puts the terminal back to how it was before. Without this, your terminal would act weird after the game closes.

---

### `was_key_pressed()`
> *Linux/Mac only*

Just checks if a key was pressed — doesn't actually read it yet. Returns 1 if yes, 0 if no. This way the game keeps moving even if you don't press anything.

---

### `reset_game()`

Resets everything back to the start. Bird goes to the middle, wall goes to the right side, score goes to 0. Called at the beginning and when you choose to play again.

---

### `draw_game_frame()`

Draws everything you see on screen. Clears the screen first, then draws the borders, the bird, the wall, the gap, and the score. This runs every single frame so the screen looks updated.

---

### `run_game()`

This is the main loop that keeps the game going. Every loop it does these things in order:
1. Draw the screen
2. Check if you pressed a key
3. Move the bird down (gravity)
4. Move the wall to the left
5. If wall goes off screen, make a new one with a random gap
6. Check if the bird crashed
7. Wait 150ms and repeat

---

### `main()`

Where the program starts. Shows the welcome screen, runs the game, shows Game Over with your score, then asks if you want to play again.

---

## Quick Summary of How It Works

The game redraws the screen around 6 times per second. Each time, the bird falls a little and the wall moves a little to the left. If you press a key, the bird jumps up. If the bird touches the wall or goes off the screen, you lose. That's basically it!
