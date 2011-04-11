#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include "Socket.h"

using namespace std;

Socket::Socket()
{
    id = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(int sid)
{
    id = sid;
}

int Socket::sendOut(void* toSend, int length)
{
    const char* msg = (const char*) toSend;
    return send(id, msg, length, 0);
}

int Socket::receive(int length, char* bufferPtr)
{
    char buffer[length];
    char* bPtr = buffer;
    int r = recv(id, bPtr, length, 0);
    bufferPtr = bPtr;
    return r;
}

int Socket::closeSocket()
{
    return close(id);
}