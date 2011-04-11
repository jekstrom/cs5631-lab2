#include "ClientSocket.h"
#include "Socket.h"
#include "headerFiles.h"
#include <sys/param.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

ClientSocket::ClientSocket()
        : Socket()
{
}

int ClientSocket::connectTo(char* port, string hostname)
{
    struct addrinfo hints;
    hints.ai_family = AF_UNSPEC;
    hints.ai_protocol = 0;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_addrlen = 0;
    hints.ai_addr = NULL;
    hints.ai_canonname = NULL;
    hints.ai_next = NULL;

    char * hostnamePtr;
    if(hostname == "localhost")
    {
        char hostName[MAXHOSTNAMELEN];
        hostnamePtr = hostName;
        gethostname(hostnamePtr, MAXHOSTNAMELEN);
    }
    else
        hostnamePtr = (char *) hostname.c_str();

    struct addrinfo * result;
    getaddrinfo(hostname.c_str(), (const char*) port, &hints, &result);

    int r = connect(this->getSid(), result->ai_addr, result->ai_addrlen);

    if(result != NULL)
        freeaddrinfo(result);
    return r;
}