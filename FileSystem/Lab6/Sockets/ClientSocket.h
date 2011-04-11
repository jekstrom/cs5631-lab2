/* 
 * File:   ClientSocket.h
 * Author: kaleb
 *
 * Created on April 5, 2011, 10:44 PM
 */

#ifndef CLIENTSOCKET_H
#define	CLIENTSOCKET_H

#include "Socket.h"
#include <string>

using namespace std;

class ClientSocket : public Socket
{
public:

    /**
     * Creates a client suitable for use by a client.
     */
    ClientSocket();

    /** Connect socket to host specified by port and hostname.
     * @pre none
     * @post socket is connected
     * @return 0 if sucessful, -1 in case of failure
     */
    virtual int connectTo(char* port, string hostname);
};

#endif	/* CLIENTSOCKET_H */

