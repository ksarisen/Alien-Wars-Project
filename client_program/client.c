#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

#include "clientGameDataStructures.h" // IMPORTANT: Please View!
#include "../server_program/gameStructures.h"
#include "playerGameInstructions.h"
#include "userAction.h"
#include "clientServerAPI.h"
#include "clientBackend.h"
#include "clientGUI.h"

struct PlayersInfo startGame() {
    connectToServer();
    struct PlayersInfo playersInfo = getPlayersInfo();
    return playersInfo;
}

// Multithread Me
void playGame(struct PlayersInfo playersInfo, struct Game * game) {
    while (!game->gameover) {
        char* userAction = getUserAction(playersInfo);
        sendToServer(userAction);
        getCurrentGameState(game);
        displayGame(game, playersInfo);
    }
}

int main(int argc, char *argv[]) {
    struct PlayersInfo playersInfo = startGame();

    struct Game* game = initGameState();

    print_how_to_play(playersInfo);

    playGame(playersInfo, game); 
}