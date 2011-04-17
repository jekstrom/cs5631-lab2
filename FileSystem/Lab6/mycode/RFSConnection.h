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

        return result;
    }

    string listDirectory()
    {
        const String METHOD("MethodName");
        const String DIR("dir");

        const String LIST_DIR("ListDir");        

        Message msg;

        msg.AddString(METHOD, LIST_DIR);
        cout << "Message made." << endl;

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);
        cout << "Message flattened." << endl;

        int size = sizeof(buffer);
        if(0 > send(sid, (const char*) &size, sizeof(int), 0))
        {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return "";
        }
        cout << "Message size sent." << endl;

        if(0 > send(sid, (const char*) buffer, size, 0))
        {
            cout << "Error: could not send message. errno = " << errno << endl;
            return "";
        }
        cout << "Message sent." << endl;

        if(0 > recv(sid, (char*) &size, sizeof(int), 0))
        {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return "";
        }
        cout << "Response size received." << endl;

        if(0 > recv(sid, (char*) buffer, size, 0))
        {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return "";
        }
        cout << "Response received." << endl;

        msg.Unflatten(buffer, size);
        cout << "Response unflattened." << endl;

        String dirStr("");
        msg.FindString(DIR, dirStr);
        cout << "Parameter retrieved:" << dirStr.Cstr() << endl;

        string dir = dirStr.Cstr();
        cout << "Return string set: " << dir << endl;
        return "blah";
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

