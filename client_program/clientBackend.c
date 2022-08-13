#include "clientBackend.h"

int socket_fd;
typedef struct addrinfo* addrinfo_list;
char buffer[MAXBUFFERBYTES];
addrinfo_list server_addrinfo_list;
ssize_t bytes_sent;
int bytes_received;


// return boolean when error
bool socket_error(int status, char* error_message) {
    if (status == -1) {
        perror(error_message);
        return true;
    }
    return false;
}

// exit when error
void exit_socket_error(int status, char* error_message) {
    if (status == -1) {
        perror(error_message);
        exit(1);
    }
}

// set to IPv4, TCP, flags=AI_PASSIVE, respectively
struct addrinfo init_server_hints() {
    struct addrinfo server_hints;
    memset(&server_hints, 0, sizeof server_hints);
    server_hints.ai_family = AF_INET;
    server_hints.ai_socktype = SOCK_STREAM;
    server_hints.ai_flags = AI_PASSIVE;
    return server_hints;
}

// create a client socket and connect to the server
void connectServer() {
    
    struct addrinfo server_hints = init_server_hints();

    //int getaddrinfo_status = getaddrinfo(NULL, SERVER_PORT, &server_hints, &server_addrinfo_list);
    // test connecting
    int getaddrinfo_status = getaddrinfo(SERVER_ADDR, SERVER_PORT, &server_hints, &server_addrinfo_list);
    //24.207.13.89
    exit_socket_error(getaddrinfo_status, "getaddrinfo()");
    
    // Setup Socket to Server Application
    for (struct addrinfo* server_addrinfo = server_addrinfo_list; server_addrinfo != NULL; server_addrinfo = server_addrinfo->ai_next) {
        socket_fd = socket(server_addrinfo->ai_family, server_addrinfo->ai_socktype, server_addrinfo->ai_protocol);
        if (socket_error(socket_fd, "socket()")) {
            continue;
        }
        int connect_status = connect(socket_fd, server_addrinfo->ai_addr, server_addrinfo->ai_addrlen);
        if (socket_error(connect_status, "connect()")) {
            continue;
        }
        break;
    }
    
    freeaddrinfo(server_addrinfo_list);
    
   /*
    printf("before connection\n");
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = inet_addr("192.168.250.1/24");
    
    servaddr.sin_addr.s_addr = inet_addr("174.7.36.77");
    servaddr.sin_port = htons(8080);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0){
        printf("connection with server failed");
        exit(0);
    }
    printf("after connection\n");
    */
   /*
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = inet_addr("192.168.250.2");
    //172.20.10.4
    servaddr.sin_addr.s_addr = inet_addr("172.20.10.4");
    servaddr.sin_port = htons(6954);
    
    printf("test before socket\n");

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_error(socket_fd, "socket()")){
        printf("socket failed");
        exit(-1);
    }
    
    printf("test before connect\n");
    
    int connect_error = connect(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(socket_error(connect_error, "connect()")){
        printf("connection with server failed");
        exit(-1);
    }
    printf("connect done\n");
    */
}

// send command to the server
void sendAction (char* ret) {
    bytes_sent = send(socket_fd, ret, 6, 0);

    // send error handle
    socket_error(bytes_sent, "send()");
}

// receive the game state
char* recvState () {
    memset(buffer, 0, MAXBUFFERBYTES);
    bytes_received = recv(socket_fd, buffer, MAXBUFFERBYTES-1, 0);
    
    // recv error handle
    socket_error(bytes_received, "recv()");

    return buffer;
}

// close socket
void closeSocket() {
    close(socket_fd);
}