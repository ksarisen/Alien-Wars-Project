#include "clientServerAPI.h"


// startGame() functions

void connectToServer() {
    // TODO: connectToServer 
    connectServer(); // by Yosup
    printf("connected to server\n");
}

struct PlayersInfo getPlayersInfo() {
    // assume after connect, server sends one msg
    // by knowing who to send
    // send in the order of player, ally, enemy 1 and 2
    // for example, 2, 1, 3, 4; if the player is assigned 2
    char* playersInfoMsg = recvState();
    struct PlayersInfo playersInfo = convertPlayersInfoMsg(playersInfoMsg); 

    // TEMPORARY TESTING CODE 
    /*
    struct PlayersInfo playersInfo;
    playersInfo.player = '1';
    playersInfo.ally   = '2';
    playersInfo.enemy1 = '3';
    playersInfo.enemy2 = '4';
    */
    
    // by Yosup
    // by Yosup
    // by Yosup
    return playersInfo; 
}


// playGame() functions

void sendToServer(char* userAction) {
    // TODO Implement
    // sendToServer(userAction); 
    sendAction(userAction); // by Yosup
    printf("User Action %s has been sent to server\n", userAction);
}

struct GameState getCurrentGameState() {
    // TODO Implement
    // char* gameStateMsg = recvFromServer(); 
    char* gameStateMsg = recvState(); // by Yosup
    // struct GameState currentGameState = convertGameStateMsg(gameStateMsg);

    // TEMPORARY TESTING CODE
    struct GameState currentGameState;
    currentGameState.gameOver = true;
    currentGameState.gameOverMsg = "test from client server api\n";
    return currentGameState;
}

// TODO Implement
void displayGameState(struct GameState currentGameState) {
    printf("Future displayGameState\n");
}
