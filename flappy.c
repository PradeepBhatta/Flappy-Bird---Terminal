#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// platform-specific headers
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
    #include <sys/select.h>
    #include <sys/time.h>
#endif

// game screen size
#define WIDTH 50
#define HEIGHT 15

// size of gap in obstacle
#define GAP_SIZE 4

// bird position (up/down)
int bird_y;

// obstacle position (left/right)
int obstacle_x;

// where the gap starts
int gap_start;

// score
int score;

// game state (0 = running, 1 = over)
int game_over;


// ----------- LINUX INPUT FIX (BEGINNER COMMENTED) -----------
#ifndef _WIN32

// store old terminal settings
struct termios old_settings;

// allow instant key press (no Enter needed)
void enable_instant_input() {
    tcgetattr(0, &old_settings);

    struct termios new_settings = old_settings;

    // turn off line buffering and echo
    new_settings.c_lflag &= ~(ICANON | ECHO);

    new_settings.c_cc[VMIN] = 0;
    new_settings.c_cc[VTIME] = 0;

    tcsetattr(0, TCSANOW, &new_settings);
}

// restore terminal back to normal
void restore_input() {
    tcsetattr(0, TCSANOW, &old_settings);
}

// check if a key is pressed
int key_pressed() {
    fd_set set;
    struct timeval time = {0, 0};

    FD_ZERO(&set);
    FD_SET(0, &set);

    return select(1, &set, NULL, NULL, &time) > 0;
}

#endif
// -----------------------------------------------------------


// reset everything before game starts
void start_new_game() {
    bird_y = HEIGHT / 2;
    obstacle_x = WIDTH - 5;
    gap_start = HEIGHT / 2;

    score = 0;
    game_over = 0;

    srand(time(NULL)); // random every time
}


// draw game screen
void draw_screen() {

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    // top border
    printf("+");
    for (int i = 0; i < WIDTH; i++) printf("-");
    printf("+\n");

    for (int row = 0; row < HEIGHT; row++) {
        printf("|");

        for (int col = 0; col < WIDTH; col++) {

            // draw bird
            if (col == 10 && row == bird_y) {
                printf("@");
            }

            // draw obstacle
            else if (col == obstacle_x) {
                if (row >= gap_start && row < gap_start + GAP_SIZE) {
                    printf(" "); // gap
                } else {
                    printf("#"); // wall
                }
            }

            // show score
            else if (row == 0 && col == 1) {
                printf("Score:%d", score);
                col += 6 + (score >= 10);
            }

            else {
                printf(" ");
            }
        }

        printf("|\n");
    }

    // bottom border
    printf("+");
    for (int i = 0; i < WIDTH; i++) printf("-");
    printf("+\n");

    printf("Press ANY KEY to jump | Q = Quit\n");
}


// main game loop
void play_game() {

#ifndef _WIN32
    enable_instant_input();
#endif

    while (!game_over) {

        draw_screen();

        // input
#ifdef _WIN32
        if (_kbhit())
#else
        if (key_pressed())
#endif
        {
#ifdef _WIN32
            char key = _getch();
#else
            char key = getchar();
#endif

            if (key == 'q' || key == 'Q') {
                game_over = 1;
            } else {
                bird_y -= 2; // jump
            }
        }

        // gravity
        bird_y += 1;

        // move obstacle
        obstacle_x--;

        // reset obstacle
        if (obstacle_x < 0) {
            obstacle_x = WIDTH - 1;
            gap_start = rand() % (HEIGHT - GAP_SIZE - 1);
            score++;
        }

        // check collision with top/bottom
        if (bird_y < 0 || bird_y >= HEIGHT) {
            game_over = 1;
        }

        // check collision with wall
        if (obstacle_x == 10) {
            if (bird_y < gap_start || bird_y >= gap_start + GAP_SIZE) {
                game_over = 1;
            }
        }

        // speed control
#ifdef _WIN32
        Sleep(150);
#else
        usleep(150000);
#endif
    }

#ifndef _WIN32
    restore_input();
#endif
}


// main function
int main() {

    char choice;

    do {
        start_new_game();

#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        printf("\n=== FLAPPY BIRD ===\n\n");
        printf("Bird falls automatically\n");
        printf("Press key to jump\n\n");
        printf("Press Enter to start...");
        getchar();

        play_game();

#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        printf("\n*** GAME OVER ***\n");
        printf("Score: %d\n\n", score);
        printf("Play again? (y/n): ");
        scanf(" %c", &choice);

        while (getchar() != '\n');

    } while (choice == 'y' || choice == 'Y');

    printf("\nThanks for playing!\n");
    return 0;
}