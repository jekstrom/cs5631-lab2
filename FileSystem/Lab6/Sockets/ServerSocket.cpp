#include "ServerSocket.h"
#include "Socket.h"
#include "headerFiles.h"
#include <sys/param.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

ServerSocket::ServerSocket()
        : Socket()
{
}

int ServerSocket::bindTo(char* port)
{
    struct addrinfo hints;
    hints.ai_family = AF_INET;
    hints.ai_protocol = 0;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_addrlen = 0;
    hints.ai_addr = NULL;
    hints.ai_canonname = NULL;
    hints.ai_next = NULL;

    struct addrinfo * result;
    getaddrinfo(NULL, port, &hints, &result);

    int r = bind(this->getSid(), result->ai_addr, result->ai_addrlen);

    socklen_t addressLen = sizeof(address);
    getsockname(this->getSid(), (struct sockaddr *) &address, &addressLen);
    freeaddrinfo(result);

    int yes = 1;
    setsockopt(this->getSid(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    return r;
}

int ServerSocket::listenUp()
{
    return listen(this->getSid(), MAXCON);
}

Socket ServerSocket::acceptRequest()
{
    struct sockaddr_in* addrPtr;
    socklen_t len = (socklen_t) sizeof(struct sockaddr_in);
    int s = accept(this->getSid(), (struct sockaddr *) addrPtr, &len);
    return Socket(s);
}