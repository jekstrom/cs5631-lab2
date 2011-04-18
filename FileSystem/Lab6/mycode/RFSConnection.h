/* 
 * File:   RFSConnection.h
 * Author: kaleb
 *
 * Created on April 17, 2011, 1:14 PM
 */

#ifndef RFSCONNECTION_H
#define	RFSCONNECTION_H

#include "../muscle/system/SetupSystem.h"
#include "../muscle/message/Message.h"
#include "../muscle/util/String.h"
#include <iostream>
#include "headerFiles.h"

using namespace muscle;
using namespace std;

class RFSConnection
{
public:
    CompleteSetupSystem css;

    RFSConnection(int sid)
    {
        String method("MethodName");
        this->sid = sid;
    }

    int openFile(String filename, String mode)
    {
        const String METHOD("MethodName");
        const String FILENAME("Filename");
        const String MODE("OpenMode");
        const String FD("fd");

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

    int deleteFile(String filename)
    {
        const String METHOD("MethodName");
        const String FILENAME("Filename");
        const String RESULT("result");        

        const String DELETE_FILE("OpenMode");

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
        const String DELETE_FILE("OpenMode");

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
        cout << "Received message " << methodStr.Cstr() << endl;

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

