#include "TicTacToe.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Function to initialize the game
void initialize_game(GameState *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->board[i][j] = ' ';
        }
    }
    pthread_mutex_init(&game->board_mutex, NULL);
    pthread_cond_init(&game->turn_cond, NULL);
    game->current_player = 0; // 0 for human, 1 for AI
    game->game_over = 0;
    srand(time(NULL)); // Seed the random number generator
}

// Function for human player
void *human_player(void *arg) {
    GameState *game = (GameState *)arg;
    int row, col;

    while (1) {
        pthread_mutex_lock(&game->board_mutex);

        while (game->current_player != 0 && !game->game_over) {
            pthread_cond_wait(&game->turn_cond, &game->board_mutex);
        }

        if (game->game_over) {
            pthread_mutex_unlock(&game->board_mutex);
            break;
        }

        display_board(game);
        printf("Enter your move (row and column, separated by space, e.g., 1 1): ");
        scanf("%d %d", &row, &col);
        row--;
        col--;

        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || game->board[row][col] != ' ') {
            printf("Invalid move. Please enter valid row and column numbers. Try again.\n");
            pthread_cond_signal(&game->turn_cond);
            pthread_mutex_unlock(&game->board_mutex);
            continue;
        }

        make_move(game, row, col, 'X');

        if (check_winner(game)) {
            printf("Human player wins!\n");
            game->game_over = 1;
        } else if (is_draw(game)) {
            printf("The game is a draw.\n");
            game->game_over = 1;
        }

        game->current_player = 1;
        pthread_cond_signal(&game->turn_cond);
        pthread_mutex_unlock(&game->board_mutex);

        if (game->game_over) {
            display_board(game);
            break;
        }
    }

    return NULL;
}

// Function for AI player
void *ai_player(void *arg) {
    GameState *game = (GameState *)arg;
    int best_row, best_col;

    while (1) {
        pthread_mutex_lock(&game->board_mutex);

        while (game->current_player != 1 && !game->game_over) {
            pthread_cond_wait(&game->turn_cond, &game->board_mutex);
        }

        if (game->game_over) {
            pthread_mutex_unlock(&game->board_mutex);
            break;
        }

        // Find a random move for AI
        if (find_random_move(game, &best_row, &best_col)) {
            make_move(game, best_row, best_col, 'O');
        }

        if (check_winner(game)) {
            printf("AI player wins!\n");
            game->game_over = 1;
        } else if (is_draw(game)) {
            printf("The game is a draw.\n");
            game->game_over = 1;
        }

        game->current_player = 0;
        pthread_cond_signal(&game->turn_cond);
        pthread_mutex_unlock(&game->board_mutex);

        if (game->game_over) {
            display_board(game);
            break;
        }
    }

    return NULL;
}

// Display the current board
void display_board(GameState *game) {
    system("clear"); // Use "cls" on Windows
    printf("Current board:\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf(" %c ", game->board[i][j]);
            if (j < BOARD_SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < BOARD_SIZE - 1) printf("---+---+---\n");
    }
}

// Check if there is a winner
int check_winner(GameState *game) {
    // Check rows, columns, and diagonals
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (game->board[i][0] != ' ' &&
            game->board[i][0] == game->board[i][1] &&
            game->board[i][1] == game->board[i][2]) {
            return 1;
        }
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (game->board[0][i] != ' ' &&
            game->board[0][i] == game->board[1][i] &&
            game->board[1][i] == game->board[2][i]) {
            return 1;
        }
    }
    if (game->board[0][0] != ' ' &&
        game->board[0][0] == game->board[1][1] &&
        game->board[1][1] == game->board[2][2]) {
        return 1;
    }
    if (game->board[0][2] != ' ' &&
        game->board[0][2] == game->board[1][1] &&
        game->board[1][1] == game->board[2][0]) {
        return 1;
    }
    return 0;
}

// Check if the game is a draw
int is_draw(GameState *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] == ' ') {
                return 0;
            }
        }
    }
    return 1;
}

// Make a move on the board
void make_move(GameState *game, int row, int col, char symbol) {
    game->board[row][col] = symbol;
}

// Find a random move for AI
int find_random_move(GameState *game, int *best_row, int *best_col) {
    int empty_spots[BOARD_SIZE * BOARD_SIZE][2];
    int count = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] == ' ') {
                empty_spots[count][0] = i;
                empty_spots[count][1] = j;
                count++;
            }
        }
    }

    if (count > 0) {
        int rand_index = rand() % count;
        *best_row = empty_spots[rand_index][0];
        *best_col = empty_spots[rand_index][1];
        return 1;
    }

    return 0;
}

// Cleanup game resources
void cleanup_game(GameState *game) {
    pthread_mutex_destroy(&game->board_mutex);
    pthread_cond_destroy(&game->turn_cond);
}
