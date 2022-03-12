#include "connectfour.h"

int exit_key = 'q';
int arrow_start = 27;
int arrow_left = 68;
int arrow_right = 67;
int space_bar = 32;

int log_fd = 1;

static const char players[2] = {'x', 'o'};

void    write_log(char *str, ...) {
    va_list args;
    va_start(args, str);
    vdprintf(log_fd, str, args);
    va_end(args);
}

int**   create_map() {
    int count = 0;
    int **map = NULL;

    map = malloc(sizeof(int *) * MAP_SIZE_Y);
    while (count < MAP_SIZE_Y) {
        map[count] = malloc(sizeof(int) * MAP_SIZE_X);
        count += 1;
    }
    return map;
}

void    free_map(int **map) {
    int count = 0;

    while (count < MAP_SIZE_Y) {
        free(map[count]);
        count += 1;
    }
    free(map);
}

char     detectLineX(int **map, int x, int y) {
    char p = map[y][x];
    int lineX = x;
    int matching = 0;

    if (!p) return -1;
    while ((lineX < x + 4) && (lineX < MAP_SIZE_X)) {
        if (map[y][lineX] != p) {
            return -1;
        }
        matching += 1;
        lineX += 1;
    }
    return matching == 4 ? p : -1;
}

char     detectLineY(int **map, int x, int y) {
    char p = map[y][x];
    int lineY = y;
    int matching = 0;

    if (!p) return -1;
    while ((lineY > y - 4) && lineY) {
        if (map[lineY][x] != p) {
            return -1;
        }
        matching += 1;
        lineY -= 1;
    }
    return matching == 4 ? p : -1;

}

char     detectDiag(int **map, int x, int y) {
    char p = map[y][x];
    int lineY = y;
    int lineX = x;
    int matching = 0;
    if (!p) return -1;
    while ((lineY > y - 4) && lineY && (lineX < x + 4) && (lineX < MAP_SIZE_X)) {
        if (map[lineY][lineX] != p) {
            return -1;
        }
        matching += 1;
        lineY -= 1;
        lineX += 1;
    }
    return matching == 4 ? p : -1;
}

char    isWinner(int **map) {
    int y = 0;
    int x = 0;
    char winner = -1;

    while (y < MAP_SIZE_Y) {
        x = 0;
        while (x < MAP_SIZE_X) {
            if ((winner = detectLineX(map, x, y)) != -1) {
                return winner;
            }
            if ((winner = detectLineY(map, x, y)) != -1) {
                return winner;
            }
            if ((winner = detectDiag(map, x, y)) != -1) {
                return winner;
            }
            x += 1;
        }
        y += 1;
    }
    return -1;
}

void    print_map(int **map) {
    int countX = 0;
    int countY = 0;

    while (countY < MAP_SIZE_Y) {
        countX = 0;
        while (countX < MAP_SIZE_X) {
            char c = map[countY][countX] ? map[countY][countX] : '_';
            printw("%c", c);
            countX += 1;
        }
        printw("\n");
        countY += 1;
    }
}

int     getYPos(int **map, int x, int y) {
    if (y == -1) return y;
    if (!map[y][x]) return y;
    return getYPos(map, x, y - 1);
}

void    move_cursor(int key, int *x, int *y) {
    int c;
    if (key == arrow_start) {
        getch();
        c = getch();
        if (c == arrow_right) {
            *x = *x + 1;
            if (*x < MAP_SIZE_X) {
                move(*y, *x);
            } else {
                *x = MAP_SIZE_X - 1;
            }
        }
        if (c == arrow_left) {
            *x = *x - 1;
            if (*x >= 0) {
                move(*y, *x);
            } else {
                *x = 0;
            }
        }
    }
}

char    place_item(int key, int **map, int x, int y, int *turn) {
    char winner = -1;

    if (key == space_bar) {
        char item = players[*turn % 2];
        int itemY = getYPos(map, x, y - 1);
        if (itemY != -1) {
            map[itemY][x] = item;
        }
        clear();
        print_map(map);
        move(y, x);
        if ((winner = isWinner(map)) != -1) {
            return winner;
        }
        *turn += 1;
    }
    return -1;
}

char    game_loop(int **map) {
    print_map(map);
    int key = 0;
    int x = 0;
    int y = MAP_SIZE_Y;
    char winner = -1;
    int turn = 0;

    while (key != exit_key) {
        key = getch();
        if ((winner = place_item(key, map, x, y, &turn)) != -1) {
            return winner;
        }
        move_cursor(key, &x, &y);
    }
    return -1;
} 

void    init_curses() {
    initscr();
    noecho();
}

void    stop_curses() {
    endwin();
}

int     main(void) {
    int **map = NULL;
    char winner = -1;

    log_fd = open("../log.txt", O_RDWR);
    init_curses();
    map = create_map();
    winner = game_loop(map);
    stop_curses();
    free_map(map);
    if (winner != -1) {
        printf("winner is %c\n", winner);
    }
    close(log_fd);
    return 0;
}
