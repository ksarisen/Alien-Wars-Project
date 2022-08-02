#include <stdio.h>
#include <errno.h> // Errors 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <poll.h>
#include "gameStructures.h"


#include <sys/types.h>
#include <sys/socket.h> 
#include <netdb.h> // getaddrinfo, gai_strerror

#define BACKLOG 6
#define FLAGS 0
#define SERVER_PORT "6969"
#define BUFFER_SIZE 8192

void exit_if_error(int status, char* error_message) {
    if (status == -1) {
        perror(error_message);
        exit(1);
    }

}

bool is_socket_error(int status, char* error_message) {
    if (status == -1) {
        perror(error_message);
        return true;
    }
    return false;
}

struct addrinfo init_server_hints() {
    struct addrinfo server_hints;
    memset(&server_hints, 0, sizeof server_hints);
    server_hints.ai_family = AF_INET;
    server_hints.ai_socktype = SOCK_STREAM;
    server_hints.ai_flags = AI_PASSIVE; // pass NULL to nodename
    return server_hints;
}

struct addrinfo * init_server_addr_list() {
    struct addrinfo hints = init_server_hints();
    struct addrinfo * server_addr_list;

    int getaddrinfo_status = getaddrinfo(NULL, SERVER_PORT, &hints, &server_addr_list);
    exit_if_error(getaddrinfo_status, "getaddrinfo()");

    return server_addr_list;
}

void addSocketInPoll(struct pollfd *fds[], int newfd, int *fd_count){
    (*fds)[*fd_count].fd = newfd;
    (*fds)[*fd_count].events = POLLIN;
    (*fd_count)++;
}

void deleteSocketInPoll(struct pollfd pfds[], int i, int *fd_count){
    pfds[i] = pfds[*fd_count-1];
    (*fd_count)--;
}


int main() {

    // server_addr_list 
    struct addrinfo * server_addr_list = init_server_addr_list();

    // socket 
    int socketfd = socket(PF_INET, SOCK_STREAM, 0); 
    exit_if_error(socketfd, "socket()");
    char buffer[BUFFER_SIZE];

    // bind socket and server_addr
    for(struct addrinfo * server_addr = server_addr_list; server_addr != NULL; server_addr = server_addr->ai_next) {
        int bind_status = bind(socketfd, server_addr->ai_addr, server_addr->ai_addrlen);
        if (is_socket_error(bind_status, "bind()")) {
            continue;
        }
        break; // if we get here, we must have connected successfully
    }
    
    freeaddrinfo(server_addr_list);
    
    // listen()
    int listen_status = listen(socketfd, 20);
    exit_if_error(listen_status, "listen()");

    
    //setup poll structure (should accept 4 player connections on top of default listener)
    int currServerConnections = 0;
    struct pollfd* serverSockets = malloc(sizeof(struct pollfd) * NUM_OF_PLAYERS);


    struct sockaddr_storage clientaddr;

    //initialize Game
    int bytesSent;
    struct Game* game = initGameState();

    //accept connections to 4 clients before proceeding into game loop
    while(currServerConnections != NUM_OF_PLAYERS) {
        socklen_t addrlen = sizeof(clientaddr);
        int newSocket = accept(socketfd, (struct sockaddr*)&clientaddr, &addrlen);
        if(newSocket == -1){
            perror("ERROR: failed to accept connection");
        }
        else{
            addSocketInPoll(&serverSockets, newSocket, currServerConnections);
            //Todo: create function to send a message to the client who just joined ("hello player x!")
        }
    }

    //Todo: make a process here that sends the data to clients every x seconds.

    //use poll to scan for events on any of the connections. Enter recieved data into game.
    while(true) {
        int numberOfEvents = poll(serverSockets, NUM_OF_PLAYERS, -1);
        if(numberOfEvents == -1){
            perror("ERROR: failed at poll");
            return -1;
        }
        for(int i = 0; i < currServerConnections; i++){
            if(serverSockets[i].revents & POLLIN){
                int inputBytes = recv(serverSockets[i].fd, buffer, sizeof(buffer), 0);
                int sender_sock = serverSockets[i].fd;
                if(inputBytes <= 0){
                    perror("ERROR: failed at recv in poll loop");
                    close(serverSockets[i].fd);
                    deleteSocketInPoll(serverSockets, i, &currServerConnections);
                }
                else if(inputBytes == 0){
                    printf("Connection to socket %d closed", serverSockets[i].fd);
                    close(serverSockets[i].fd);
                    deleteSocketInPoll(serverSockets, i, &currServerConnections);
                }
                //Todo: Here's where we need to use the function that actually interprets data!
            }
        }
    }
}