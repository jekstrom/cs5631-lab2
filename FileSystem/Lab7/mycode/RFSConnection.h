/* 
 * File:   RFSConnection.h
 * Author: kaleb
 *
 * Created on April 17, 2011, 1:14 PM
 */

#ifndef RFSCONNECTION_H
#define	RFSCONNECTION_H

#include "system/SetupSystem.h"
#include "message/Message.h"
#include "util/String.h"
#include <iostream>
#include "headerFiles.h"
#include "FileSystemHeaders.h"
#include "OpenFileTable.h"

using namespace muscle;
using namespace std;

/**
 * This class acts as an RPC interface between the server and clients, using
 * Muscle messages to pass data.
 */
class RFSConnection
{
public:
    CompleteSetupSystem css;

    /**
     * Creates a new RFSConnection which communicates through a given socket.
     * @param sid The socket descriptor of the connection's socket
     */
    RFSConnection(int sid)
    {
        this->sid = sid;
        dirPtr = NULL;
    }

    /**
     * Creates a new RFSConnection linked to a file directory.
     * @param sid The socket descriptor of the connection's socket
     * @param dirPtr A pointer to the directory the connection will use
     */
    RFSConnection(int sid, FileDirectory* dirPtr)
    {
        this->sid = sid;
        this->dirPtr = dirPtr;
    }

    /**
     * Sends a request to open a file for wither reading or writing.
     * and receives the server's response via muscle messages
     * @param filename The name of the file to open
     * @param mode One of either "read" or "write"
     * @return The now open file's FD (server currently returns 42)
     */
    int openFile(String filename, String mode)
    {
        const String METHOD("MethodName");
        const String FILENAME("Filename");
        const String MODE("OpenMode");
        const String FD("fd");

        const String QUIT("quit");
        const String OPEN_FILE("OpenFile");

        Message msg;

        msg.AddString(METHOD, OPEN_FILE);        
        msg.AddString(FILENAME, filename);
        msg.AddString(MODE, mode);

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);

        int size = sizeof(buffer);
        if(0 > send(sid, (const char*) &size, sizeof(int), 0))
        {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return -1;
        }

        if(0 > send(sid, (const char*) buffer, size, 0))
        {
            cout << "Error: could not send message. errno = " << errno << endl;
            return -1;
        }
        cout << "Sending message OPEN_FILE." <<endl;

        if(0 > recv(sid, (char*) &size, sizeof(int), 0))
        {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return -1;
        }

        if(0 > recv(sid, (char*) buffer, size, 0))
        {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return -1;
        }

        msg.Unflatten((const uint8*) buffer, size);

        int fd = 0;
        msg.FindInt32(FD, 0, (int32*) &fd);
        cout << "Received message \"" << fd << "\"." <<endl;

        return fd;
    }

    /**
     * Sends a request to close a particular open file.
     * @param fd the file descriptor of the file to close
     * @return 0 if the file was successfully closed, -1 otherwise
     */
    int closeFile(int fd)
    {
        const String METHOD("MethodName");
        const String FD("fd");
        const String RESULT("result");

        const String CLOSE_FILE("CloseFile");

        Message msg;

        msg.AddString(METHOD, CLOSE_FILE);
        msg.AddInt32(FD, fd);

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);

        int size = sizeof(buffer);
        if(0 > send(sid, (const char*) &size, sizeof(int), 0))
        {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return -1;
        }

        if(0 > send(sid, (const char*) buffer, size, 0))
        {
            cout << "Error: could not send message. errno = " << errno << endl;
            return -1;
        }
        cout << "Sending message CLOSE_FILE." << endl;

        if(0 > recv(sid, (char*) &size, sizeof(int), 0))
        {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return -1;
        }

        if(0 > recv(sid, (char*) buffer, size, 0))
        {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return -1;
        }

        msg.Unflatten(buffer, size);

        int result = 0;
        msg.FindInt32(RESULT, 0, (int32*) &result);

        cout << "Received message \"" << result << "\"." << endl;
        return result;
    }

    /**
     * Sends a request for a listing of the file directory,
     * and receives the response from the server (the directory as a std::string)
     * @return A string representing the directory
     */
    string listDirectory()
    {
        const String METHOD("MethodName");
        const String DIR("dir");

        const String LIST_DIR("ListDir");        

        Message msg;

        msg.AddString(METHOD, LIST_DIR);

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);

        int size = sizeof(buffer);
        if(0 > send(sid, (const char*) &size, sizeof(int), 0))
        {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return "";
        }

        if(0 > send(sid, (const char*) buffer, size, 0))
        {
            cout << "Error: could not send message. errno = " << errno << endl;
            return "";
        }
        cout << "Sending message LIST_DIR." << endl;

        int size2 = 0;
        if(0 > recv(sid, (char*) &size2, sizeof(int), 0))
        {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return "";
        }

        uint8 buffer2[size2];
        if(0 > recv(sid, (char*) buffer2, size2, 0))
        {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return "";
        }


        msg.Unflatten(buffer2, size2);

        String dirStr("");
        msg.FindString(DIR, dirStr);

        string dir = dirStr.Cstr();
        return dir;
    }

    /**
     * Sends a request to delete a particular file.
     * @param filename The name of the file to delete
     * @return 0 if the file was deleted successfully, else -1
     */
    int deleteFile(String filename)
    {
        const String METHOD("MethodName");
        const String FILENAME("Filename");
        const String RESULT("result");        

        const String DELETE_FILE("DeleteFile");

        Message msg;

        msg.AddString(METHOD, DELETE_FILE);
        msg.AddString(FILENAME, filename);

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);

        int size = sizeof(buffer);
        if(0 > send(sid, (const char*) &size, sizeof(int), 0))
        {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return -1;
        }

        if(0 > send(sid, (const char*) buffer, size, 0))
        {
            cout << "Error: could not send message. errno = " << errno << endl;
            return -1;
        }
        cout << "Sending message DELETE_FILE." << endl;

        if(0 > recv(sid, (char*) &size, sizeof(int), 0))
        {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return -1;
        }

        if(0 > recv(sid, (char*) buffer, size, 0))
        {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return -1;
        }

        msg.Unflatten(buffer, size);

        int result = 0;
        msg.FindInt32(RESULT, 0, (int32*) &result);
        cout << "Received message \"" << result << "\"." << endl;

        return result;
    }

    /**
     * Sends a message indicating that the client will disconnct.
     * @return -1 if there was a failure in sending the message, else 0
     */
    int quit()
    {
        const String METHOD("MethodName");
        const String QUIT("quit");

        Message msg;

        msg.AddString(METHOD, QUIT);

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);

        cout << "Sending message QUIT." << endl;
        int size = sizeof(buffer);
        if(0 > send(sid, (const char*) &size, sizeof(int), 0))
        {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return -1;
        }

        if(0 > send(sid, (const char*) buffer, size, 0))
        {
            cout << "Error: could not send message. errno = " << errno << endl;
            return -1;
        }
        cout << "Message sent." << endl;

        return 0;
    }

    /**
     * Handles client requests sent by using the other methods of RFSConnection.
     * Will abort if the calling RFSConnection is not linked to a directory.
     * @return -1 in the case of an error or if no directory is available,
     *         0 for success, or 1 if a "quit" message was received.
     */
    int handleRequest()
    {
        // Check that a directory is available.
        if(dirPtr == NULL)
        {
            cout << "Error: handling requests requires directory access." << endl;
            return -1;
        }

        const String METHOD("MethodName");
        const String FILENAME("Filename");
        const String MODE("OpenMode");
        const String FD("fd");
        const String DIR("dir");
        const String RESULT("result");
        const String QUIT("quit");

        const String OPEN_FILE("OpenFile");
        const String CLOSE_FILE("CloseFile");
        const String LIST_DIR("ListDir");
        const String DELETE_FILE("DeleteFile");

        Message msg;
        int size = 0;

        if(0 > recv(sid, (char*) &size, sizeof(int), 0))
        {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return -1;
        }

        uint8 recvBuffer[size];

        if(0 > recv(sid, (char*) recvBuffer, size, 0))
        {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return -1;
        }

        msg.Unflatten(recvBuffer, size);

        String methodStr("");
        msg.FindString(METHOD, methodStr);
        cout << "Received request: " << methodStr.Cstr() << endl;

        if(methodStr == OPEN_FILE)
        {
            int fd = 42;
            String filename("");
            String mode("");

            msg.FindString(FILENAME, filename);
            msg.FindString(MODE, mode);
            cout << "Filename: " << filename.Cstr() << ", mode: ";
            cout << mode.Cstr() << endl;

            msg.AddInt32(FD, fd);

            uint8 buffer[msg.FlattenedSize()];
            msg.Flatten(buffer);

            int size = sizeof(buffer);
            if(0 > send(sid, (const char*) &size, sizeof(int), 0))
            {
                cout << "Error: could not send message size. errno = " << errno << endl;
                return -1;
            }

            if(0 > send(sid, (const char*) buffer, size, 0))
            {
                cout << "Error: could not send message. errno = " << errno << endl;
                return -1;
            }
        }
        else if(methodStr == CLOSE_FILE)
        {
            int result = 0;
            int fd = 0;

            msg.FindInt32(FD, 0, (int32*) &fd);
            cout << "File desriptor: " << fd << endl;

            msg.AddInt32(RESULT, result);

            uint8 buffer[msg.FlattenedSize()];
            msg.Flatten(buffer);

            int size = sizeof(buffer);
            if(0 > send(sid, (const char*) &size, sizeof(int), 0))
            {
                cout << "Error: could not send message size. errno = " << errno << endl;
                return -1;
            }

            cout << "Message sent to client" << endl;

            if(0 > send(sid, (const char*) buffer, size, 0))
            {
                cout << "Error: could not send message. errno = " << errno << endl;
                return -1;
            }
        }
        else if(methodStr == LIST_DIR)
        {
            String dirList("Start of FileDirectory\nEnd of FileDirectory");

            msg.AddString(DIR, dirList);

            uint8 buffer[msg.FlattenedSize()];
            msg.Flatten(buffer);

            int size = sizeof(buffer);
            if(0 > send(sid, (const char*) &size, sizeof(int), 0))
            {
                cout << "Error: could not send message size. errno = " << errno << endl;
                return -1;
            }

            if(0 > send(sid, (const char*) buffer, size, 0))
            {
                cout << "Error: could not send message. errno = " << errno << endl;
                return -1;
            }
            cout << "Message sent to client" << endl;
        }
        else if(methodStr == DELETE_FILE)
        {
            int result = 0;
            String filename("");

            msg.FindString(FILENAME, filename);
            cout << "Filename: " << filename.Cstr() << endl;

            msg.AddInt32(RESULT, result);

            uint8 buffer[msg.FlattenedSize()];
            msg.Flatten(buffer);

            int size = sizeof(buffer);
            if(0 > send(sid, (const char*) &size, sizeof(int), 0))
            {
                cout << "Error: could not send message size. errno = " << errno << endl;
                return -1;
            }

            if(0 > send(sid, (const char*) buffer, size, 0))
            {
                cout << "Error: could not send message. errno = " << errno << endl;
                return -1;
            }
            cout << "Message sent to client" << endl;
        }
        else if(methodStr == QUIT)
        {
            return 1;
        }
        else
        {
            cout << "Received unrecognized request." << endl;
            return -1;
        }
    }

private:

    int sid;

    FileDirectory* dirPtr;
};

#endif	/* RFSCONNECTION_H */

