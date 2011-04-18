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

    RFSConnection(int sid)
    {
        this->sid = sid;
    }

    /**
     * openFile sends the method name OpenFile to the server, as well as the
     * parameters necessary for the OpenFile function (filename and mode),
     * and receives the server's response via muscle messages
     * @param filename the name of the file to open
     * @param mode read or write
     * @return the file's FD (currently always returns 42)
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
     * closeFile sends the method name CloseFile with the parameter fd to the
     * server, and receives the response (success or failure)
     * @param fd the file descriptor of the file to close
     * @return the result of closing the file
     */
    int closeFile(int fd)
    {
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
        const String DELETE_FILE("OpenMode");

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
     * listDirectory sends method name ListDir to the server with no parameters,
     * and receives the response from the server (the directory as a std::string)
     * @return the directory as returned from the server
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
     * deleteFile sends the method name OpenMode to the server with the filename
     * as a parameter, and receives the result of deleting the file from the server
     * (success or failure)
     * @param filename the filename of the file to close
     * @return result of deleting the file as returned by the server
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
     * quit sends the message quit to the server with no parameters.
     * @return -1 if a failure in sending the message
     */
    int quit()
    {
        const String METHOD("MethodName");
        const String QUIT("quit");

        Message msg;

        msg.AddString(METHOD, QUIT);

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
        cout << "Sending message QUIT." << endl;
    }

    /**
     * handleRequest handles any muscle message received and sends the result
     * back to the client
     * @return -1 if failure in receiving or sending the message.
     */
    int handleRequest()
    {
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
            String dirList("Start of Directory\nEnd of Directory");

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
};

#endif	/* RFSCONNECTION_H */

