/* 
 * File:   Socket.h
 * Author: Kaleb
 *
 * Created on April 5, 2011, 10:27 PM
 */

#ifndef SOCKET_H
#define	SOCKET_H

using namespace std;

class Socket
{
    public:

    /**
     * Creates a socket object following TCP/IP.
     */
    Socket();

    /**
     * Wraps the given socket id in a Socket object.
     * @param sid The id of an open socket
     */
    Socket(int sid);

    // Default destructor
    ~Socket();

    /** Sends the content pointed to by toSend which has size length.
     * @pre socket is connected
     * @post none
     * @return number of bytes sent, or -1 in case of failure
     */
    virtual int sendOut(void* toSend, int length);

    /** Receive a maximum of "length" bytes and store them in bufferPtr
     * @pre socket is connected
     * @post received message is contained in *bufferPtr
     * @return number of bytes received, or -1 in case of failure
     */
    virtual int receive(int length, char* bufferPtr);

    /** Closes the socket.
     * @pre socket is open
     * @post socket is closed
     * @return 0 if successful, -1 in case of failure
     */
    virtual int closeSocket();

    /**
     * Retrieves the socket descriptor.
     * @return The socket descriptor
     */
    int getSid()
    {
        return id;
    }

private:

    /**
     * The socket descriptor.
     */
    int id;
};

#endif	/* SOCKET_H */

