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
#include "../muscle/besupport/support/MuscleSupport.h"
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
class RFSConnection {
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
     * @param dirPtr A pointer to the directory the connection will use
     */
    RFSConnection(int sid, FileDirectory* dirPtr, Disk* diskPtr) {
        this->sid = sid;
        this->dirPtr = dirPtr;
        this->diskPtr = diskPtr;
    }

    /**
     * Sends a request to open a file for wither reading or writing.
     * and receives the server's response via muscle messages
     * @param filename The name of the file to open
     * @param mode One of either "read" or "write"
     * @return The now open file's FD (server currently returns 42)
     */
    int openFile(String filename, String mode) {
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

        int size = sizeof (buffer);
        if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return -1;
        }

        if (0 > send(sid, (const char*) buffer, size, 0)) {
            cout << "Error: could not send message. errno = " << errno << endl;
            return -1;
        }
        cout << "Sending message OPEN_FILE." << endl;

        if (0 > recv(sid, (char*) &size, sizeof (int), 0)) {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return -1;
        }

        if (0 > recv(sid, (char*) buffer, size, 0)) {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return -1;
        }

        msg.Unflatten((const uint8*) buffer, size);

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

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);

        int size = sizeof (buffer);
        if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return -1;
        }

        if (0 > send(sid, (const char*) buffer, size, 0)) {
            cout << "Error: could not send message. errno = " << errno << endl;
            return -1;
        }
        cout << "Sending message CLOSE_FILE." << endl;

        if (0 > recv(sid, (char*) &size, sizeof (int), 0)) {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return -1;
        }

        if (0 > recv(sid, (char*) buffer, size, 0)) {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return -1;
        }

        msg.Unflatten(buffer, size);

        int result = 0;
        msg.FindInt32(RESULT, 0, (int32*) & result);

        cout << "Received message \"" << result << "\"." << endl;
        return result;
    }

    /**
     * Sends a request for a listing of the file directory,
     * and receives the response from the server (the directory as a std::string)
     * @return A string representing the directory
     */
    string listDirectory() {
        const String METHOD("MethodName");
        const String DIR("dir");

        const String LIST_DIR("ListDir");

        Message msg;

        msg.AddString(METHOD, LIST_DIR);

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);

        int size = sizeof (buffer);
        if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return "";
        }

        if (0 > send(sid, (const char*) buffer, size, 0)) {
            cout << "Error: could not send message. errno = " << errno << endl;
            return "";
        }
        cout << "Sending message LIST_DIR." << endl;

        int size2 = 0;
        if (0 > recv(sid, (char*) &size2, sizeof (int), 0)) {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return "";
        }

        uint8 buffer2[size2];
        if (0 > recv(sid, (char*) buffer2, size2, 0)) {
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
    int deleteFile(String filename) {
        const String METHOD("MethodName");
        const String FILENAME("Filename");
        const String RESULT("result");

        const String DELETE_FILE("DeleteFile");

        Message msg;

        msg.AddString(METHOD, DELETE_FILE);
        msg.AddString(FILENAME, filename);

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);

        int size = sizeof (buffer);
        if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return -1;
        }

        if (0 > send(sid, (const char*) buffer, size, 0)) {
            cout << "Error: could not send message. errno = " << errno << endl;
            return -1;
        }
        cout << "Sending message DELETE_FILE." << endl;

        if (0 > recv(sid, (char*) &size, sizeof (int), 0)) {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return -1;
        }

        if (0 > recv(sid, (char*) buffer, size, 0)) {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return -1;
        }

        msg.Unflatten(buffer, size);

        int result = 0;
        msg.FindInt32(RESULT, 0, (int32*) & result);
        cout << "Received message \"" << result << "\"." << endl;

        return result;
    }

    /**
     * this function reads (readAmt) bytes data from disk into buf.
     * @param fd the filedescriptor of the file to read from.
     * @param readAmt the amount of bytes to read.
     * @param buf the data read.
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

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);

        int size = sizeof (buffer);
        if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return "";
        }

        if (0 > send(sid, (const char*) buffer, size, 0)) {
            cout << "Error: could not send message. errno = " << errno << endl;
            return "";
        }
        cout << "Sending message READ." << endl;

        int size2 = 0;
        if (0 > recv(sid, (char*) &size2, sizeof (int), 0)) {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return "";
        }

        uint8 buffer2[size2];
        if (0 > recv(sid, (char*) buffer2, size2, 0)) {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return "";
        }

        msg.Unflatten(buffer2, size2);

        int bytesRead = 0;
        int result = 0;

        msg.FindInt32(BYTESREAD, bytesRead);
        msg.FindData(DATA, B_ANY_TYPE, buf, &result);

        return bytesRead;
    }

    /**
     * this function writes (writeAmt) bytes of buf to disk
     * @param fd the file descriptor of the file to write to.
     * @param writeAmt the amount of bytes to write.
     * @param buf the data to write to the file.
     * @return number of bytes written.
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

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);

        int size = sizeof (buffer);
        if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return "";
        }

        if (0 > send(sid, (const char*) buffer, size, 0)) {
            cout << "Error: could not send message. errno = " << errno << endl;
            return "";
        }
        cout << "Sending message WRITE." << endl;

        int size2 = 0;
        if (0 > recv(sid, (char*) &size2, sizeof (int), 0)) {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return "";
        }

        uint8 buffer2[size2];
        if (0 > recv(sid, (char*) buffer2, size2, 0)) {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return "";
        }

        msg.Unflatten(buffer2, size2);

        int bytesWritten = 0;
        int result = 0;

        msg.FindInt32(BYTESWRITTEN, bytesWritten);

        return bytesWritten;
    }

    bool fileExists(string filename) {
        const String METHOD("MethodName");
        const String FILENAME("Filename");
        const String FILE_EXISTS("file_exists");
        const String RESULT("result");
        const String DIR("dir");

        Message msg;

        msg.AddString(METHOD, FILE_EXISTS);
        msg.AddString(FILENAME, filename);

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);

        int size = sizeof (buffer);
        if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return "";
        }

        if (0 > send(sid, (const char*) buffer, size, 0)) {
            cout << "Error: could not send message. errno = " << errno << endl;
            return "";
        }
        cout << "Sending message FILE_EXISTS." << endl;

        int size2 = 0;
        if (0 > recv(sid, (char*) &size2, sizeof (int), 0)) {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return "";
        }

        uint8 buffer2[size2];
        if (0 > recv(sid, (char*) buffer2, size2, 0)) {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return "";
        }

        int result = 0;
        msg.FindInt32(RESULT, result);

        return (result >= 0);
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

        uint8 buffer[msg.FlattenedSize()];
        msg.Flatten(buffer);

        cout << "Sending message QUIT." << endl;
        int size = sizeof (buffer);
        if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
            cout << "Error: could not send message size. errno = " << errno << endl;
            return -1;
        }

        if (0 > send(sid, (const char*) buffer, size, 0)) {
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
        const String DIR("dir");
        const String RESULT("result");
        const String QUIT("quit");
        const String DATA("Data");
        const String BYTESWRITTEN("byteswritten");
        const String BYTESREAD("bytesread");
        const String FILE_EXISTS("file_exists");

        const String OPEN_FILE("OpenFile");
        const String CLOSE_FILE("CloseFile");
        const String LIST_DIR("ListDir");
        const String DELETE_FILE("DeleteFile");
        const String READ("Read");
        const String WRITE("Write");

        Message msg;
        int size = 0;

        if (0 > recv(sid, (char*) &size, sizeof (int), 0)) {
            cout << "Error: could not receive response size. errno = " << errno << endl;
            return -1;
        }

        uint8 recvBuffer[size];

        if (0 > recv(sid, (char*) recvBuffer, size, 0)) {
            cout << "Error: could not receive response. errno = " << errno << endl;
            return -1;
        }

        msg.Unflatten(recvBuffer, size);

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

            int fcb = dirPtr->findFile(filename);

            if (fcb < 0) { //create the file because it was not found in directory
                file = new File(filename, true, mode, diskPtr, dirPtr);
            } else { //found file, open it
                file = new File(filename, false, mode, diskPtr, dirPtr);
            }

            //add file to open file table
            fd = oft.addEntry(file);

            msg.AddInt32(FD, fd);

            uint8 buffer[msg.FlattenedSize()];
            msg.Flatten(buffer);

            int size = sizeof (buffer);
            if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
                cout << "Error: could not send message size. errno = " << errno << endl;
                return -1;
            }

            if (0 > send(sid, (const char*) buffer, size, 0)) {
                cout << "Error: could not send message. errno = " << errno << endl;
                return -1;
            }

        } else if (methodStr == CLOSE_FILE) {
            int result = 0;
            int fd = 0;

            msg.FindInt32(FD, 0, (int32*) & fd);
            cout << "File desriptor: " << fd << endl;

            //get file from open file table corresponding to given fd
            File *file = oft.removeEntry(fd);

            delete file;

            msg.AddInt32(RESULT, result);

            uint8 buffer[msg.FlattenedSize()];
            msg.Flatten(buffer);

            int size = sizeof (buffer);
            if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
                cout << "Error: could not send message size. errno = " << errno << endl;
                return -1;
            }

            cout << "Message sent to client" << endl;

            if (0 > send(sid, (const char*) buffer, size, 0)) {
                cout << "Error: could not send message. errno = " << errno << endl;
                return -1;
            }

        } else if (methodStr == LIST_DIR) {
            String dirList("Start of FileDirectory\n\n");

            list<Entry> entryList = dirPtr->listEntries();
            for (list<Entry>::iterator i = entryList.begin(); i != entryList.end(); i++) {
                dirList += i->fcbNum;
                dirList += " ";
                dirList += i->name;
                
                dirList += "\n";
            }

            dirList += "\n End of Directory \n";

            msg.AddString(DIR, dirList);

            uint8 buffer[msg.FlattenedSize()];
            msg.Flatten(buffer);

            int size = sizeof (buffer);
            if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
                cout << "Error: could not send message size. errno = " << errno << endl;
                return -1;
            }

            if (0 > send(sid, (const char*) buffer, size, 0)) {
                cout << "Error: could not send message. errno = " << errno << endl;
                return -1;
            }
            cout << "Message sent to client" << endl;

        } else if (methodStr == DELETE_FILE) {
            int result = 0;
            String filename("");

            msg.FindString(FILENAME, filename);
            cout << "Filename: " << filename.Cstr() << endl;

            int fcb = dirPtr->findFile(filename);
            if (fcb < 0) { //couldn't find file, doesn't exist
                result = -1;
                cout << "Error: Could not find file " << filename << endl;
            } else { //found file, delete it
                File *file = new File(filename, false, false, diskPtr, dirPtr);

                if (!file->close()) {
                    cout << "Error: Ejecting warp core!" << endl;
                    result = -1;
                }

                //remove from open file table

                if (!file->deleteFile())
                    cout << "Error: Could not delete file " << filename << endl;
                result = 1;
            }

            msg.AddInt32(RESULT, result);

            uint8 buffer[msg.FlattenedSize()];
            msg.Flatten(buffer);

            int size = sizeof (buffer);
            if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
                cout << "Error: could not send message size. errno = " << errno << endl;
                return -1;
            }

            if (0 > send(sid, (const char*) buffer, size, 0)) {
                cout << "Error: could not send message. errno = " << errno << endl;
                return -1;
            }
            cout << "Message sent to client" << endl;

        } else if (methodStr == READ) {
            int fd = 0;
            int bytesToRead = 0;

            msg.FindInt32(FD, fd);
            msg.FindInt32(BYTESREAD, bytesToRead);

            cout << "FD: " << fd << endl;

            File *file = oft.getFilePtr(fd);

            void* buf = NULL;
            int bytesRead = file->read(buf, bytesToRead);

            msg.Clear(true);
            msg.AddInt32(BYTESREAD, bytesRead);
            msg.AddData(DATA, B_ANY_TYPE, buf, bytesRead);

            uint8 buffer[msg.FlattenedSize()];
            msg.Flatten(buffer);

            int size = sizeof (buffer);
            if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
                cout << "Error: could not send message size. errno = " << errno << endl;
                return -1;
            }

            if (0 > send(sid, (const char*) buffer, size, 0)) {
                cout << "Error: could not send message. errno = " << errno << endl;
                return -1;
            }
            cout << "Message sent to client" << endl;

        } else if (methodStr == WRITE) {
            int fd = 0;
            int bytesToWrite = 0;
            int result = 0;
            void* buf = NULL; //buffer to hold data from write method

            msg.FindInt32(FD, fd);
            msg.FindInt32(BYTESWRITTEN, bytesToWrite);
            msg.FindData(DATA, B_ANY_TYPE, buf, &result); //put data from write into buf

            cout << "FD: " << fd << endl;

            File *file = oft.getFilePtr(fd);

            int bytesWritten = file->write(buf, bytesToWrite);

            msg.Clear(true);
            msg.AddInt32(BYTESWRITTEN, bytesWritten);
            msg.AddData(DATA, B_ANY_TYPE, buf, bytesWritten);

            uint8 buffer[msg.FlattenedSize()];
            msg.Flatten(buffer);

            int size = sizeof (buffer);
            if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
                cout << "Error: could not send message size. errno = " << errno << endl;
                return -1;
            }

            if (0 > send(sid, (const char*) buffer, size, 0)) {
                cout << "Error: could not send message. errno = " << errno << endl;
                return -1;
            }
            cout << "Message sent to client" << endl;

        } else if (methodStr == FILE_EXISTS) {
            int result = 0;

            string filename = "";
            msg.FindString(FILENAME, filename);

            cout << "Filename: " << filename << endl;

            result = dirPtr->findFile(filename);

            msg.Clear(true);
            msg.AddInt32(RESULT, result);

            uint8 buffer[msg.FlattenedSize()];
            msg.Flatten(buffer);

            int size = sizeof (buffer);
            if (0 > send(sid, (const char*) &size, sizeof (int), 0)) {
                cout << "Error: could not send message size. errno = " << errno << endl;
                return -1;
            }

            if (0 > send(sid, (const char*) buffer, size, 0)) {
                cout << "Error: could not send message. errno = " << errno << endl;
                return -1;
            }
            cout << "Message sent to client" << endl;

        } else if (methodStr == QUIT) {
            return 1;

        } else {
            cout << "Received unrecognized request." << endl;
            return -1;
        }
    }

private:

    int sid;

    FileDirectory* dirPtr;

    Disk* diskPtr;

    OpenFileTable oft;
};

#endif	/* RFSCONNECTION_H */

