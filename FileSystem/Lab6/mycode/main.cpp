#include "system/SetupSystem.h"
#include "message/Message.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

using namespace muscle;
using namespace std;

int main(int argc, char** argv) {
CompleteSetupSystem css; // first line of main

    int cmdIndex = 1;
    char* cmd = argv[cmdIndex];
    boolean isServer = false;

    string arg(cmd);

    if(arg == "-s")
        isServer = true;
    else if(arg == "-c")
        isServer = false;
    else
    {
        cout << "Use -s for a server or -c for a client." << endl;
        return EXIT_SUCCESS;
    }

    cmdIndex++;
    cmd = argv[cmdIndex];

    if(isServer)
    {
        // read in additional command-line arguments for server
        char* port = "4242";
        int maxConnections = 3;
        while(cmd != NULL)
        {
            arg = string(cmd);
            if(arg == "--port")
            {
                cmdIndex++;
                cmd = argv[cmdIndex];
                port = cmd;
            }
            else if(arg == "--maxcon")
            {
                cmdIndex++;
                cmd = argv[cmdIndex];
                maxConnections = atoi((const char*) cmd);
            }
            eles
                cout << arg << " is not a valid command." << endl;
        }

        // create IPv4 TCP/IP socket
        int sid = socket(AF_INET, SOCK_STREAM, 0);

        // bind socket to given port
        struct addrinfo hints;
        hints.ai_family = AF_INET;
        hints.ai_protocol = 0;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        hints.ai_addrlen = 0;
        hints.ai_addr = NULL;
        hints.ai_canonname = NULL;
        hints.ai_next = NULL;

        struct addrinfo* result;
        getaddrinfo(NULL, port, &hints, &result);

        if(-1 == bind(this->getSid(), result->ai_addr, result->ai_addrlen))
        {
            cout << "Error: failed in binding socket. errno = " << errno << endl;
            return EXIT_FAILURE;
        }
        else
            cout << "Server bound to port " << port;

        struct sockaddr_in address;
        socklen_t addressLen = sizeof(address);
        getsockname(sid, (struct sockaddr *) &address, &addressLen);
        freeaddrinfo(result);

        int yes = 1;
        setsockopt(sid, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        // start listening for connections
        if(-1 == listen(sid, maxConnections))
        {
            cout << "Error: listening on socket failed. errno = " << errno << endl;
            return EXIT_FAILURE;
        }
        else
            cout << ".\nServer listening for connections." << endl;

        // accept connection request
        struct sockaddr_in* addrPtr;
        socklen_t len = (socklen_t) sizeof(struct sockaddr_in);
        int clientSid = accept(this->getSid(), (struct sockaddr *) addrPtr, &len);

        if(0 > clientSid)
        {
            cout << "Error: failed to accept connection request. errno = " << errno << endl;
            return EXIT_FAILURE;
        }
        else
            cout << "Connection request accepted.\n" << endl;

        // *spawn client handler thread here*

        // handler loop for testing
        int length = 64;
        char buffer[length];
        char* bPtr = buffer;
        const char* msg = "Message received.";
        while(true)
        {
            if(0 > recv(clientSid, bPtr, length, 0))
                cout << "Error: failed in receiving from client. errno = " << errno << endl;
            else
            {
                cout << "Received message: " << bPtr << endl;

                if(0 > send(clientSid, msg, sizeof(msg), 0))
                    cout << "Error: failed in sending to client. errno = " << errno << endl;
                else
                    cout << "Sent acknowledgement of message.\n" << endl ;
            }
        }
    }
    else
    {
        // read in additional command-line arguments for client
        char* port = "4242";
        string hostname = "localhost";
        while(cmd != NULL)
        {
            arg = string(cmd);
            if(arg == "--port")
            {
                cmdIndex++;
                cmd = argv[cmdIndex];
                port = cmd;
            }
            else if(arg == "--server")
            {
                cmdIndex++;
                cmd = argv[cmdIndex];
                hostname = string(cmd);
            }
            eles
                cout << arg << " is not a valid command." << endl;

            // create IPv4 TCP/IP socket
            int sid = socket(AF_INET, SOCK_STREAM, 0);

            // connect to server
            struct addrinfo hints;
            hints.ai_family = AF_UNSPEC;
            hints.ai_protocol = 0;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = 0;
            hints.ai_addrlen = 0;
            hints.ai_addr = NULL;
            hints.ai_canonname = NULL;
            hints.ai_next = NULL;

            char* hostnamePtr;
            if(hostname == "localhost")
            {
                char hostName[MAXHOSTNAMELEN];
                hostnamePtr = hostName;
                gethostname(hostnamePtr, MAXHOSTNAMELEN);
            }
            else
                hostnamePtr = (char*) hostname.c_str();

            struct addrinfo* result;
            getaddrinfo(hostname.c_str(), (const char*) port, &hints, &result);

            int r = connect(sid, result->ai_addr, result->ai_addrlen);

            if(result != NULL)
                freeaddrinfo(result);
        }
    }
}
