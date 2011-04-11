/* 
 * File:   ServerSocket.h
 * Author: kaleb
 *
 * Created on April 5, 2011, 10:38 PM
 */

#ifndef SERVERSOCKET_H
#define	SERVERSOCKET_H

#include "Socket.h"
#include "headerFiles.h"

using namespace std;

class ServerSocket : public Socket
{
public:

    /**
     * The maximum number of connections allowed.
     */
    const static int MAXCON = 5;

    /**
     * Creates a socket suitable for use by a server.
     */
    ServerSocket();

    /** Binds the socket to the specified port; allows the OS to pick
     *  any available port if port is NULL, lets the OS choose any
     *  available network interface and address.
     * @pre none
     * @post socket is bound to specified port
     */
    virtual int bindTo(char* port);

     /** Starts listening on the socket.
     * @pre none
     * @post the socket is listening
     * @return 0 if successful, -1 otherwise
     */
    virtual int listenUp();

    /** Creates a socket for an incoming connection request.
     * @return a new socket conneted to the requesting client
     */
    virtual Socket acceptRequest();

private:

    /**
     * Contains the port number and address of the socket.
     */
    struct sockaddr_in address;
};

#endif	/* SERVERSOCKET_H */

