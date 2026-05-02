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
| `bird_y` | A number (0 to HEIGHT) | Tracks where the bird is vertically (up and down) |
| `obstacle_x` | A number (0 to WIDTH) | Tracks how far left the obstacle has moved |
| `gap_start` | A number | Tells us where the gap in the obstacle starts |
| `score` | A number starting at 0 | Increases by 1 every time you pass an obstacle |
| `game_over` | 0 or 1 | 0 = game running, 1 = game ended |
| `old_settings` | Terminal config | Saves original terminal settings (Linux/Mac only) |
| `WIDTH` | Set to 50 | How wide the game screen is |
| `HEIGHT` | Set to 15 | How tall the game screen is |
| `GAP_SIZE` | Set to 4 | How big the gap in the obstacle is |

---

## Functions — What Each One Does

### `enable_instant_input()`
> *Linux/Mac only*

Normally, the terminal waits for you to press Enter before reading input. This function disables that so key presses are detected instantly during the game.

---

### `restore_input()`
> *Linux/Mac only*

Restores the terminal back to normal after the game ends. Without this, your terminal might behave strangely.

---

### `key_pressed()`
> *Linux/Mac only*

Checks if a key was pressed without stopping the program. Returns 1 if a key is pressed, otherwise 0.

---

### `start_new_game()`

Resets everything to the starting state. The bird goes to the middle, obstacle resets to the right side, and score becomes 0.

---

### `draw_screen()`

Draws everything on the screen:
- Clears the terminal
- Draws borders
- Draws the bird `@`
- Draws the obstacle `#` with a gap
- Displays the score

Runs every frame to update the display.

---

### `play_game()`

This is the main game loop. It keeps running until the game ends.

Each loop:
1. Draw the screen
2. Check for key press
3. Apply gravity (bird goes down)
4. Move obstacle left
5. Reset obstacle when it goes off screen
6. Check collisions
7. Wait ~150ms

---

### `main()`

The starting point of the program.

- Shows welcome screen
- Starts the game
- Displays Game Over and score
- Asks if you want to play again

---

## Summary of How It Works

The game refreshes the screen multiple times per second. Each time:
- The bird slowly falls down
- The obstacle moves left
- You press a key to jump

If the bird hits the obstacle or goes off screen → game over.

That's it — simple and fun!