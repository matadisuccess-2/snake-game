#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>

#define WIDTH 40
#define HEIGHT 20
#define MAXTAIL 200

int x, y;                   // Snake head
int fruitX, fruitY;         // Fruit
int score;
int tailX[MAXTAIL], tailY[MAXTAIL];
int nTail;
char dir;                   // 'l','r','u','d'
int gameOver;               // 0 = running, 1 = over

void setup() {
    clrscr();
    gameOver = 0;
    dir = 'r';
    x = WIDTH / 2;
    y = HEIGHT / 2;

    randomize();            // Borland-specific RNG init
    fruitX = random(WIDTH);
    fruitY = random(HEIGHT);

    score = 0;
    nTail = 0;
}

void draw() {
    int i, j, k;

    clrscr();

    // top border
    for (i = 0; i < WIDTH + 2; i++) cout << "#";
    cout << "\n";

    // field
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (j == 0) cout << "#";

            if (i == y && j == x) {
                cout << "O"; // head
            } else if (i == fruitY && j == fruitX) {
                cout << "F"; // fruit
            } else {
                int printed = 0;
                for (k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        printed = 1;
                        break;
                    }
                }
                if (!printed) cout << " ";
            }

            if (j == WIDTH - 1) cout << "#";
        }
        cout << "\n";
    }

    // bottom border
    for (i = 0; i < WIDTH + 2; i++) cout << "#";
    cout << "\nScore: " << score << "\n";
    cout << "Controls: W/A/S/D to move, X to quit\n";
}

void input() {
    if (kbhit()) {
        char c = getch();
        if (c == 'a' && dir != 'r') dir = 'l';
        else if (c == 'd' && dir != 'l') dir = 'r';
        else if (c == 'w' && dir != 'd') dir = 'u';
        else if (c == 's' && dir != 'u') dir = 'd';
        else if (c == 'x') gameOver = 1;
    }
}

void logic() {
    int i;
    int prevX = tailX[0], prevY = tailY[0];
    int prev2X, prev2Y;

    // move tail
    tailX[0] = x;
    tailY[0] = y;
    for (i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // move head
    if (dir == 'l') x--;
    else if (dir == 'r') x++;
    else if (dir == 'u') y--;
    else if (dir == 'd') y++;

    // wrap-around walls (change to death on wall if you want)
    if (x >= WIDTH) x = 0;
    else if (x < 0) x = WIDTH - 1;
    if (y >= HEIGHT) y = 0;
    else if (y < 0) y = HEIGHT - 1;

    // collision with tail
    for (i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = 1;
            break;
        }
    }

    // fruit eaten
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = random(WIDTH);
        fruitY = random(HEIGHT);
        if (nTail < MAXTAIL) nTail++;
    }
}

void main() {
    setup();
    while (!gameOver) {
        draw();
        input();
        logic();
        delay(90); // control speed
    }
    cout << "\nGame Over! Final Score: " << score;
    getch();
}
