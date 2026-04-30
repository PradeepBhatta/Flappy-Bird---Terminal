#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// these headers are different depending on what OS you are using
// windows uses windows.h and conio.h
// linux/mac uses the other three
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
    #include <sys/select.h>   // needed for fd_set and select()
    #include <sys/time.h>     // needed for struct timeval
#endif

// the size of our game board (how wide and tall it is)
#define BOARD_WIDTH 50
#define BOARD_HEIGHT 15

// how big the opening in the wall is (so the bird can pass through)
#define WALL_OPENING_SIZE 4

// where the bird is on the screen (up and down position)
int bird_row;

// where the wall/pillar currently is (left and right position)
int wall_column;

// where the opening in the wall starts (up and down position)
int opening_start_row;

// the players current score (goes up each time you pass a wall)
int player_score;

// this becomes 1 when the game ends (bird hits something)
int is_game_over;


// -------------------------------------------------------
// the code below is only for linux and mac
// it helps us read keyboard input without pressing enter
// windows already has this built in so we dont need it there
// -------------------------------------------------------
#ifndef _WIN32

// we save the original terminal settings here so we can put them back later
struct termios saved_terminal_settings;

// this function changes the terminal so we can read keys one at a time
// normally the terminal waits for you to press enter before reading input
// we dont want that in a game so we turn that off here
void fix_terminal_for_game() {
    // save the current settings first
    tcgetattr(0, &saved_terminal_settings);

    // copy the settings so we can change them
    struct termios game_terminal_settings = saved_terminal_settings;

    // turn off the "wait for enter" mode and turn off showing what you type
    game_terminal_settings.c_lflag &= ~(ICANON | ECHO);

    // dont wait at all for input, just check and move on
    game_terminal_settings.c_cc[VMIN] = 0;
    game_terminal_settings.c_cc[VTIME] = 0;

    // apply these new settings
    tcsetattr(0, TCSANOW, &game_terminal_settings);
}

// this puts the terminal back to normal after the game is done
void restore_terminal_to_normal() {
    tcsetattr(0, TCSANOW, &saved_terminal_settings);
}

// this checks if the player pressed a key without actually reading it yet
// returns 1 if a key was pressed, 0 if not
int was_key_pressed() {
    fd_set watched_inputs;
    struct timeval no_wait_time = {0, 0};
    FD_ZERO(&watched_inputs);
    FD_SET(0, &watched_inputs);
    return select(1, &watched_inputs, NULL, NULL, &no_wait_time) > 0;
}

#endif
// -------------------------------------------------------
// end of linux/mac only code
// -------------------------------------------------------


// this function sets everything back to the start
// called at the beginning and when the player wants to play again
void reset_game() {
    // put bird in the middle of the screen
    bird_row = BOARD_HEIGHT / 2;

    // put the wall near the right side
    wall_column = BOARD_WIDTH - 5;

    // put the opening in the middle of the wall
    opening_start_row = BOARD_HEIGHT / 2;

    // reset the score to zero
    player_score = 0;

    // game is not over yet
    is_game_over = 0;

    // this makes the random numbers different each time we play
    srand(time(NULL));
}


// this function draws the whole game on the screen every frame
void draw_game_frame() {
    // clear the screen first so we can redraw everything fresh
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    // draw the top border of the game box
    printf("+");
    for (int col = 0; col < BOARD_WIDTH; col++) {
        printf("-");
    }
    printf("+\n");

    // now draw every row of the game one by one
    for (int row = 0; row < BOARD_HEIGHT; row++) {
        printf("|");  // left border of the box

        for (int col = 0; col < BOARD_WIDTH; col++) {

            // draw the bird at column 10 on its current row
            if (col == 10 && row == bird_row) {
                printf("@");  // @ is our bird character
            }

            // draw the wall/pillar
            else if (col == wall_column) {
                // check if this part of the wall is the opening
                if (row >= opening_start_row && row < opening_start_row + WALL_OPENING_SIZE) {
                    printf(" ");  // this is the gap, leave it empty
                } else {
                    printf("#");  // this is the solid wall
                }
            }

            // show the score at the top left corner of the board
            else if (row == 0 && col == 1) {
                printf("Score:%d", player_score);
                // skip ahead so we dont overwrite the score with spaces
                // we skip 7 characters, plus one more if score is 2 digits
                col += 6 + (player_score >= 10);
            }

            // everything else is just empty space
            else {
                printf(" ");
            }
        }

        printf("|\n");  // right border of the box
    }

    // draw the bottom border of the game box
    printf("+");
    for (int col = 0; col < BOARD_WIDTH; col++) {
        printf("-");
    }
    printf("+\n");

    // show controls at the bottom
    printf("Press ANY KEY to jump | Q = Quit\n");
}


// this is the main game loop
// it keeps running until the game is over
void run_game() {

    // fix the terminal input on linux/mac
#ifndef _WIN32
    fix_terminal_for_game();
#endif

    // keep looping until someone loses or quits
    while (is_game_over == 0) {

        // draw the current state of the game
        draw_game_frame();

        // check if the player pressed a key
#ifdef _WIN32
        if (_kbhit()) {
            char pressed_key = _getch();
            if (pressed_key == 'q' || pressed_key == 'Q') {
                is_game_over = 1;  // player wants to quit
            } else {
                bird_row -= 2;  // jump up by 2 rows
            }
        }
#else
        if (was_key_pressed()) {
            char pressed_key = getchar();
            if (pressed_key == 'q' || pressed_key == 'Q') {
                is_game_over = 1;  // player wants to quit
            } else {
                bird_row -= 2;  // jump up by 2 rows
            }
        }
#endif

        // gravity pulls the bird down one row every frame
        bird_row += 1;

        // move the wall one step to the left
        wall_column -= 1;

        // if the wall went off the left side of the screen
        if (wall_column < 0) {
            // bring a new wall in from the right side
            wall_column = BOARD_WIDTH - 1;

            // put the opening at a random height
            // we make sure the opening doesnt go off the top or bottom
            opening_start_row = rand() % (BOARD_HEIGHT - WALL_OPENING_SIZE - 1);

            // give the player a point for passing the wall
            player_score++;
        }

        // check if the bird flew off the top or bottom of the screen
        if (bird_row < 0 || bird_row >= BOARD_HEIGHT) {
            is_game_over = 1;  // bird left the screen, game over
        }

        // check if the bird hit the wall (at the same column as the wall)
        // and is NOT in the opening
        if (wall_column == 10) {
            if (bird_row < opening_start_row || bird_row >= opening_start_row + WALL_OPENING_SIZE) {
                is_game_over = 1;  // bird hit the solid part of the wall
            }
        }

        // wait a little bit before the next frame
        // this controls how fast the game runs
#ifdef _WIN32
        Sleep(150);  // 150 milliseconds
#else
        usleep(150000);  // 150000 microseconds = 150 milliseconds
#endif
    }

    // put the terminal back to normal when game ends (linux/mac)
#ifndef _WIN32
    restore_terminal_to_normal();
#endif
}


// this is where the program starts
int main() {
    // this stores whether the player wants to play again
    char play_again_choice;

    // keep playing until the player says no
    do {
        // set all the game variables back to starting values
        reset_game();

        // show the start/welcome screen
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        printf("\n\n  === FLAPPY BIRD ===\n\n");
        printf("  The bird falls down on its own!\n");
        printf("  Press any key to make it jump up\n\n");
        printf("  Press Enter to start...");
        getchar();  // wait for the player to press enter

        // start playing the game
        run_game();

        // show the game over screen with the final score
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        printf("\n\n  *** GAME OVER ***\n");
        printf("  Your Score: %d\n\n", player_score);
        printf("  Want to play again? (y/n): ");
        scanf(" %c", &play_again_choice);

        // clear leftover input from the buffer
        while (getchar() != '\n');

    } while (play_again_choice == 'y' || play_again_choice == 'Y');

    // say goodbye
    printf("\n  Thanks for playing!\n\n");
    return 0;
}