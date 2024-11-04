#include "TicTacToe.h"
#include <pthread.h>

int main() {
    GameState game;
    initialize_game(&game);

    pthread_t human_thread, ai_thread;

    // Create threads for human and AI players
    pthread_create(&human_thread, NULL, human_player, (void *)&game);
    pthread_create(&ai_thread, NULL, ai_player, (void *)&game);

    // Wait for both threads to finish
    pthread_join(human_thread, NULL);
    pthread_join(ai_thread, NULL);

    // Cleanup
    cleanup_game(&game);

    return 0;
}
