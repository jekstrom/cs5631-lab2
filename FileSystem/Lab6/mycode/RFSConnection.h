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
#include <iostream>
#include "headerFiles.h"

using namespace muscle;
using namespace std;

class RFSConnection
{
public:
    const static String METHOD("MethodName");
    const static String FILENAME("Filename");
    const static String MODE("OpenMode");
    const static String FD("fd");
    const static String DIR("dir");
    const static String RESULT("result");

    const static String OPEN_FILE("OpenFile");
    const static String CLOSE_FILE("CloseFile");
    const static String LIST_DIR("ListDir");
    const static String DELETE_FILE("OpenMode");


    RFSCOnnection(int sid)
    {
        this->sid = sid;
    }

    int openFile(string filename, string mode)
    {
        Message msg;

        msg.AddString(METHOD, OPEN_FILE);
        msg.AddString(FILENAME, filename);
        msg.AddString(MODE, mode);

        uint8* buffer[msg.FlattenSize()];
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
        msg.FindInt32(FD, 0, &fd);

        return fd;
    }

    int closeFile(int fd)
    {
        Message msg;

        msg.AddString(METHOD, CLOSE_FILE)
        msg.AddInt32(FD, fd);

        uint8* buffer[msg.FlattenSize()];
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

        int result = 0;
        msg.FindInt32(RESULT, 0, &result);

        return result;
    }

    string listDirectory()
    {
        Message msg;

        msg.AddString(METHOD, LIST_DIR);

        uint8* buffer[msg.FlattenSize()];
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

        String dirStr("");
        msg.FindString(DIR, dirStr);

        return string(dirStr.Cstr());
    }

    int deleteFile(string filename)
    {
        Message msg;

        msg.AddString(METHOD, DELETE_FILE);
        msg.AddString(FILENAME, filename);

        uint8* buffer[msg.FlattenSize()];
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

        int result = 0;
        msg.FindInt32(RESULT, 0, &result);

        return result;
    }

    bool handleRequest()
    {
        Message msg;

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

        String methodName("");
        msg.FindString(METHOD, methodName);

        string methodStr = string(methodName.Cstr());
        if(methodStr == OPEN_FILE)
        {

        }
        else if(methodStr == CLOSE_FILE)
        {

        }
        else if(methodStr == LIST_DIR)
        {

        }
        else if(methodStr == DELETE_FILE)
        {

        }
        else
        {
            cout << "Received unrecognized request." << endl;
            return false;
        }
    }

private:

    int sid;
};

#endif	/* RFSCONNECTION_H */

