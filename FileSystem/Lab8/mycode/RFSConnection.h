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
#include "../muscle/support/MuscleSupport.h"
#include <iostream>
#include "headerFiles.h"
#include "FileSystemHeaders.h"
#include "OpenFileTable.h"
#include "GlobalFileTable.h"
#include <list>

using namespace muscle;
using namespace std;

struct dirEntry
{
    string filename;
    int fcbNum;
    int startBlock;
    int endBlock;
    int numBlocks;
};

/**
 * This class acts as an RPC interface between the server and clients, using
 * Muscle messages to pass data.
 */
class RFSConnection {
private:

    int sid;

    GlobalFileTable* gftPtr;

    FileDirectory* dirPtr;

    Disk* diskPtr;

    OpenFileTable oft;

    /**
     * Sends a message stored in msg.
     * @param msg A message that has its content set
     * @return -1 in case of error, 0 otherwise
     */
    int sendMsg(Message *msg)
    {
        uint8 buffer[msg->FlattenedSize()];
        msg->Flatten(buffer);

        int size = sizeof (buffer);
        if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return -1;
        }

        if (0 > send(sid, (const char*) buffer, size, 0)) {
            cout << "Error: could not send message. errno = " << errno << endl;
            return -1;
        }

        return 0;
    }

    /**
     * Receives a message and stores it in msg.
     * @param msg A message with unimportant content
     * @return -1 in case of error, 0 otherwise
     */
    int recvMsg(Message *msg)
    {
        int size = 0;
        if (0 > recv(sid, (char*) &size, sizeof (int), 0)) {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return -1;
        }

        uint8 buffer[size];
        if (0 > recv(sid, (char*) buffer, size, 0)) {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return -1;
        }

        msg->Unflatten(buffer, size);
        return 0;
    }

    /**
     * Sends msg and receives a response into msg.
     * @param msg A message that has its content set
     * @return -1 in case of error, 0 otherwise
     */
    int sendRecv(Message *msg)
    {
//        cout << "IN SEND RCV" <<endl;
        cout << "Message in sendRecv: " << msg->GetFirstFieldNameString(B_ANY_TYPE)->Cstr() <<endl;
        uint8 buffer[msg->FlattenedSize()];
        msg->Flatten(buffer);
//        cout << "here" <<endl;
        int size = sizeof (buffer);
        if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return -1;
        }
//        cout << "here2" <<endl;
        if (0 > send(sid, (const char*) buffer, size, 0)) {
            cout << "Error: could not send message. errno = " << errno << endl;
            return -1;
        }
//        cout << "here3" <<endl;
        int size2 = 0;
        if (0 > recv(sid, (char*) &size2, sizeof (int), 0)) {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return -1;
        }
//        cout << "here4" <<endl;
        uint8 buffer2[size2];
        if (0 > recv(sid, (char*) buffer2, size2, 0)) {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return -1;
        }
//        cout << "here5" <<endl;
        msg->Unflatten(buffer2, size2);
//        cout << "Message in sendRecv: " << msg->GetFirstFieldNameString(B_ANY_TYPE)->Cstr() <<endl;
        return 0;
    }

public:
    CompleteSetupSystem css;

    /**
     * Creates a new RFSConnection which communicates through a given socket.
     * @param sid The socket descriptor of the connection's socket
     */
    RFSConnection(int sid) {
        this->sid = sid;
        dirPtr = NULL;
    }

    /**
     * Creates a new RFSConnection linked to a file directory.
     * @param sid The socket descriptor of the connection's socket
     * @param gftPtr A pointer to a the global open file table
     * @param dirPtr A pointer to the directory the connection will use
     * @param diskPtr A pointer to the disk
     */
    RFSConnection(int sid, GlobalFileTable* gftPtr, FileDirectory* dirPtr, Disk* diskPtr) {
        this->sid = sid;
        this->gftPtr = gftPtr;
        this->dirPtr = dirPtr;
        this->diskPtr = diskPtr;
    }

    /**
     * Sends a request to open a file for wither reading or writing.
     * and receives the server's response via muscle messages
     * @param filename The name of the file to open
     * @param mode One of either "read" or "write"
     * @return The now open file's FD
     */
    int openFile(String filename, String mode) {
        const String METHOD("MethodName");
        const String FILENAME("Filename");
        const String MODE("OpenMode");
        const String FD("fd");

        const String OPEN_FILE("OpenFile");
        const String READ("Read");
        const String WRITE("Write");

        Message msg;

        msg.AddString(METHOD, OPEN_FILE);
        msg.AddString(FILENAME, filename);

        if(string(mode.Cstr()) == "read")
            msg.AddString(MODE, READ);
        else if(string(mode.Cstr()) == "write")
            msg.AddString(MODE, WRITE);
        else
        {
            cout << "Mode for opening a file must be either \"read\" or \"write\"." << endl;
            return -1;
        }

        if(-1 == sendRecv(&msg))
            return -1;

        cout << "Message in openFile: " << msg.GetFirstFieldNameString(B_ANY_TYPE)->Cstr() <<endl;
        int fd = 0;
        msg.FindInt32(FD, 0, (int32*) & fd);
        cout << "Received message \"" << fd << "\"." << endl;

        return fd;
    }

    /**
     * Sends a request to close a particular open file.
     * @param fd the file descriptor of the file to close
     * @return 0 if the file was successfully closed, -1 otherwise
     */
    int closeFile(int fd) {
        const String METHOD("MethodName");
        const String FD("fd");
        const String RESULT("result");

        const String CLOSE_FILE("CloseFile");

        Message msg;

        msg.AddString(METHOD, CLOSE_FILE);
        msg.AddInt32(FD, fd);

        if(-1 == sendRecv(&msg))
            return -1;

        int result = 0;
        msg.FindInt32(RESULT, 0, (int32*) & result);

        cout << "Received message \"" << result << "\"." << endl;
        return result;
    }

    /**
     * Gets information about the contents of the remote file directory.
     * @return A list of structures containing information about directory entries;
     *         the list will be empty if an error occurs.
     */
    list<dirEntry> listDirectory() {
        const String METHOD("MethodName");
        const String FILENAME("Filename");
        const String RESULT("result");
        const String FCB("fcb");
        const String START_BLOCK("startBlock");
        const String END_BLOCK("endBlock");
        const String NUM_BLOCKS("numBlocks");


        const String LIST_DIR("ListDir");

        Message msg;

        msg.AddString(METHOD, LIST_DIR);

        sendRecv(&msg);

        // response contains number of entries in directory
        int numEntries = 0;
        msg.FindInt32(RESULT, (int32*) &numEntries);

        list<dirEntry> entryList;
        String fileName("");
        for(int i = 0; i < numEntries; i++)
        {
            // receive message containing entry information
            if(-1 == recvMsg(&msg))
                    return entryList;

            // store received information in a dirEntry
            dirEntry curEntry;
            msg.FindString(FILENAME, fileName);
            curEntry.filename = string(fileName.Cstr());
            msg.FindInt32(FCB, 0, (int32*) &curEntry.fcbNum);
            msg.FindInt32(START_BLOCK, 0, (int32*) &curEntry.startBlock);
            msg.FindInt32(END_BLOCK, 0, (int32*) &curEntry.endBlock);
            msg.FindInt32(NUM_BLOCKS, 0, (int32*) &curEntry.numBlocks);

            entryList.push_back(curEntry);
        }

        return entryList;
    }

    /**
     * Sends a request to delete a particular file.
     * @param filename The name of the file to delete
     * @return 0 if the file was deleted successfully, else -1
     */
    int deleteFile(String filename) {
        const String METHOD("MethodName");
        const String FILENAME("Filename");
        const String RESULT("result");

        const String DELETE_FILE("DeleteFile");

        Message msg;

        msg.AddString(METHOD, DELETE_FILE);
        msg.AddString(FILENAME, filename);

        if(-1 == sendRecv(&msg))
            return -1;

        int result = 0;
        msg.FindInt32(RESULT, 0, (int32*) &result);
        cout << "Received message \"" << result << "\"." << endl;

        return result;
    }

    /**
     * Reads readAmt bytes of data from a file into buf.
     * @param fd The file descriptor of the file to read from
     * @param readAmt The number of bytes to read
     * @param buf A buffer for the read data
     * @return number of bytes read.
     */
    int readFile(int fd, int readAmt, void* buf) {
        const String METHOD("MethodName");
        const String FD("fd");
        const String DATA("Data");
        const String BYTESREAD("bytesread");

        const String READ("Read");

        Message msg;
        msg.AddString(METHOD, READ);
        msg.AddInt32(FD, fd);
        msg.AddInt32(BYTESREAD, readAmt);

        if(-1 == sendRecv(&msg))
            return -1;

        int bytesRead = 0;
        int result = 0;
        void* dataBuf;

        msg.FindInt32(BYTESREAD, (int32*) &bytesRead);
        msg.FindData(DATA, B_ANY_TYPE, (const void**) &dataBuf, (uint32*) &result);
        memcpy(buf, dataBuf, result);                

        return bytesRead;
    }

    /**
     * Writes writeAmt bytes of buf to a file.
     * @param fd The file descriptor of the file to write to
     * @param writeAmt The amount of bytes to write
     * @param buf The data to write to the file
     * @return The number of bytes written, or -1 in case of error
     */
    int writeFile(int fd, int writeAmt, void* buf) {
        const String METHOD("MethodName");
        const String FD("fd");
        const String DATA("Data");
        const String BYTESWRITTEN("byteswritten");

        const String WRITE("Write");

        Message msg;

        msg.AddString(METHOD, WRITE);
        msg.AddInt32(FD, fd);
        msg.AddInt32(BYTESWRITTEN, writeAmt);
        msg.AddData(DATA, B_ANY_TYPE, buf, writeAmt);
//        cout << "Buf in writeFile: " << (const char*) buf << endl;
        if(-1 == sendRecv(&msg))
            return -1;

        int bytesWritten = 0;
        int result = 0;

        msg.FindInt32(BYTESWRITTEN, (int32*) &bytesWritten);

        return bytesWritten;
    }

    /**
     * Determines if a file exists.
     * @param filename The name of the file to search for
     * @return 1 if the file exists, 0 if it does not, -1 in case of error
     */
    int fileExists(String filename) {
        const String METHOD("MethodName");
        const String FILENAME("Filename");
        const String FILE_EXISTS("file_exists");
        const String RESULT("result");        

        Message msg;

        msg.AddString(METHOD, FILE_EXISTS);
        msg.AddString(FILENAME, filename);

        if(-1 == sendRecv(&msg))
            return -1;

        int result = 0;
        msg.FindInt32(RESULT, (int32*) &result);

        if(-1 == result)        
            return 0;        
        else        
            return 1;        
    }

    /**
     * Sends a message indicating that the client will disconnct.
     * @return -1 if there was a failure in sending the message, else 0
     */
    int quit() {
        const String METHOD("MethodName");
        const String QUIT("quit");

        Message msg;

        msg.AddString(METHOD, QUIT);

        if(-1 == sendMsg(&msg))
            return -1;

        cout << "Message sent." << endl;

        return 0;
    }

    /**
     * Handles client requests sent by using the other methods of RFSConnection.
     * Will abort if the calling RFSConnection is not linked to a directory.
     * @return -1 in the case of an error or if no directory is available,
     *         0 for success, or 1 if a "quit" message was received.
     */
    int handleRequest() {
        // Check that a directory is available.
        if (dirPtr == NULL) {
            cout << "Error: handling requests requires directory access." << endl;
            return -1;
        }

        const String METHOD("MethodName");
        const String FILENAME("Filename");
        const String MODE("OpenMode");
        const String FD("fd");
        const String RESULT("result");
        const String QUIT("quit");
        const String DATA("Data");
        const String BYTESWRITTEN("byteswritten");
        const String BYTESREAD("bytesread");
        const String FILE_EXISTS("file_exists");
        const String FCB("fcb");
        const String START_BLOCK("startBlock");
        const String END_BLOCK("endBlock");
        const String NUM_BLOCKS("numBlocks");

        const String OPEN_FILE("OpenFile");
        const String CLOSE_FILE("CloseFile");
        const String LIST_DIR("ListDir");
        const String DELETE_FILE("DeleteFile");
        const String READ("Read");
        const String WRITE("Write");

        Message msg;
        if(-1 == recvMsg(&msg))
                return -1;

        String methodStr("");
        msg.FindString(METHOD, methodStr);
        cout << "Received request: " << methodStr.Cstr() << endl;

        if (methodStr == OPEN_FILE) {
            int fd = -1;
            String filename("");
            String mode("");

            msg.FindString(FILENAME, filename);
            msg.FindString(MODE, mode);
            cout << "Filename: " << filename.Cstr() << ", mode: ";
            cout << mode.Cstr() << endl;

            File *file = NULL;

            int fcb = dirPtr->findFile(string(filename.Cstr()));

            bool readAccess = true;
            if(mode == WRITE)
                readAccess = false;

            if (fcb < 0) { //create the file because it was not found in directory
                file = new File(string(filename.Cstr()), true, readAccess, diskPtr, dirPtr);
            } else { //found file, open it
                file = new File(string(filename.Cstr()), false, readAccess, diskPtr, dirPtr);
            }

            //add file to open file table
            fd = oft.addEntry(file);

            msg.AddInt32(FD, fd);

            if(-1 == sendMsg(&msg))
                return -1;

        } else if (methodStr == CLOSE_FILE) {
            int result = 0;
            int fd = 0;

            msg.FindInt32(FD, 0, (int32*) & fd);
            cout << "File desriptor: " << fd << endl;

            //remove file from open file table corresponding to given fd
            oft.removeEntry(fd);

            msg.AddInt32(RESULT, result);

            if(-1 == sendMsg(&msg))
                return -1;

        } else if (methodStr == LIST_DIR) {
            list<Entry> entryList = dirPtr->listEntries();

            int numEntries = entryList.size();
            msg.AddInt32(RESULT, numEntries);

            if(-1 == sendMsg(&msg))
                return -1;

            cout << "Directory size sent to client." << endl;
            
            for (list<Entry>::iterator i = entryList.begin(); i != entryList.end(); i++)
            {
                msg.Clear(true);
                
                // TODO: add information from directory entry to msg
                Block fcb = Block(i->fcb, diskPtr);
                msg.AddString(FILENAME, i->name.c_str());
                msg.AddInt32(FCB, i->fcb);
                msg.AddInt32(START_BLOCK, fcb.getPointer(1)); // use named constants?
                msg.AddInt32(END_BLOCK, fcb.getPointer(2));
                msg.AddInt32(NUM_BLOCKS, fcb.getPointer(3));

                if(-1 == sendMsg(&msg))
                    return -1;
            }
            
        } else if (methodStr == DELETE_FILE) {
            int result = 0;
            String filename("");

            msg.FindString(FILENAME, filename);
            cout << "Filename: " << filename.Cstr() << endl;

            int fcb = dirPtr->findFile(string(filename.Cstr()));
            if (fcb < 0) { //couldn't find file, doesn't exist
                result = -1;
                cout << "Error: Could not find file " << string(filename.Cstr()) << endl;
            } else { //found file, delete it
                File *file = new File(string(filename.Cstr()), false, false, diskPtr, dirPtr);

                if (!file->close()) {
                    cout << "Error: Ejecting warp core!" << endl;
                    result = -1;
                }

                //remove from open file table

                if (!file->deleteFile())
                    cout << "Error: Could not delete file " << filename.Cstr() << endl;
                result = 1;
            }

            msg.AddInt32(RESULT, result);

            if(-1 == sendMsg(&msg))
                return -1;

            cout << "Message sent to client" << endl;

        } else if (methodStr == READ) {
            int fd = 0;
            int bytesToRead = 0;

            msg.FindInt32(FD, (int32*) &fd);
            msg.FindInt32(BYTESREAD, (int32*) &bytesToRead);
            msg.Clear(true);

            cout << "FD: " << fd << endl;

            File *file = oft.getFilePtr(fd);

            if(file != NULL)
            {
                cout << "File found in table." << endl;
                char buf[bytesToRead];
                int bytesRead = file->read(buf, bytesToRead);
                cout << "Read " << bytesRead << " bytes from ";
                cout << file->getName() << ": " << buf << endl;

                msg.AddInt32(BYTESREAD, (int32) bytesRead);
                msg.AddData(DATA, B_ANY_TYPE, (const void*) buf, bytesRead);
            }
            else
            {
                cout << "File not found in table." << endl;
                int bytesRead = -1;
                msg.AddInt32(BYTESREAD, (int32) &bytesRead);
            }

            if(-1 == sendMsg(&msg))
                return -1;

            cout << "Message sent to client" << endl;

        } else if (methodStr == WRITE) {
            int fd = 0;
            int bytesToWrite = 0;
            int result = 0;
            void* buf = NULL; //buffer to hold data from write method

            msg.FindInt32(FD, (int32*) &fd);
            msg.FindInt32(BYTESWRITTEN, (int32*) &bytesToWrite);
            msg.FindData(DATA, B_ANY_TYPE, (const void**) &buf, (uint32*) &result); //put data from write into buf
            cout << "Buffer after find data: " << (const char*) buf <<endl;
            //msg.Clear(false);
            
            cout << "FD: " << fd << endl;

            File *file = oft.getFilePtr(fd);
            Message msg2;
            if(file != NULL)
            {
                cout << "File found in table." << endl;
                cout << "Buffer we are writing: " << (const char*) buf << endl;
                int bytesWritten = file->write(buf, bytesToWrite);

                msg2.AddInt32(BYTESWRITTEN, bytesWritten);
                if (msg2.AddData(DATA, B_ANY_TYPE, buf, (uint32) bytesWritten)
                        != B_NO_ERROR)
                    cout << "SERVER: Error: Couldn't add data!" << endl;
            }
            else
            {
                cout << "File not found in table." << endl;
                int bytesWritten = -1;
                msg2.AddInt32(BYTESWRITTEN, bytesWritten);
            }

            if(-1 == sendMsg(&msg2))
                    return -1;

            cout << "Message sent to client" << endl;

        } else if (methodStr == FILE_EXISTS) {
            int result = 0;

            String filename("");
            msg.FindString(FILENAME, filename);

            cout << "Filename: " << filename.Cstr() << endl;

            result = dirPtr->findFile(string(filename.Cstr()));

            msg.Clear(true);
            msg.AddInt32(RESULT, result);

            if(-1 == sendMsg(&msg))
                return -1;

            cout << "Message sent to client: " << result << endl;

        } else if (methodStr == QUIT) {
            return 1;

        } else {
            cout << "Received unrecognized request." << endl;
            return -1;
        }

        return 0;
    }
};

#endif	/* RFSCONNECTION_H */

