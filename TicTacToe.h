#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <pthread.h>

// Constants
#define BOARD_SIZE 3

// Data Structures
typedef struct {
    char board[BOARD_SIZE][BOARD_SIZE];
    pthread_mutex_t board_mutex;
    pthread_cond_t turn_cond;
    int current_player;
    int game_over;
} GameState;

// Function Declarations
void initialize_game(GameState *game);
void *human_player(void *arg);
void *ai_player(void *arg);
int check_winner(GameState *game);
int is_draw(GameState *game);
void display_board(GameState *game);
void make_move(GameState *game, int row, int col, char symbol);
int find_random_move(GameState *game, int *best_row, int *best_col);
void cleanup_game(GameState *game);

#endif // TICTACTOE_H