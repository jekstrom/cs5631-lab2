#include "../muscle/system/SetupSystem.h"
#include "../muscle/message/Message.h"
#include <iostream>
#include "headerFiles.h"
#include <pthread.h>
#include "RFSConnection.h"
#include "FileSystemHeaders.h"

using namespace muscle;
using namespace std;

void* connectionThread(void* dataPtr);
void* inputThread(void* dataPtr);
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  threadsFree = PTHREAD_COND_INITIALIZER;
pthread_mutex_t test1a_mutex = PTHREAD_MUTEX_INITIALIZER;
bool* threadDone;
int sid;
FileDirectory* dirPtr;
GlobalFileTable gft;

struct connectionThreadParameter
{
    int socID;
    int threadNumber;
    FileDirectory* dirPtr;
    Disk* diskPtr;
};

int main(int argc, char** argv) {
    CompleteSetupSystem css; // first line of main

    int cmdIndex = 1;
    char* cmd = argv[cmdIndex];
    bool isServer = false;

    string arg(cmd);

    if(arg == "-s")
        isServer = true;
    else if(arg == "-c")
        isServer = false;
    else
    {
        cout << "Use -s for a server or -c for a client." << endl;
        return EXIT_SUCCESS;
    }

    cmdIndex++;
    cmd = argv[cmdIndex];

    if(isServer)
    {
        // read in additional command-line arguments for server
        
        char* port = "4242";
        int maxConnections = 3;
        char* diskName = "testdisk";
        bool formatDisk = false;
        while(cmd != NULL)
        {
            arg = string(cmd);
            if(arg == "--port")
            {
                cmdIndex++;
                cmd = argv[cmdIndex];
                port = cmd;
            }
            else if(arg == "--maxcon")
            {
                cmdIndex++;
                cmd = argv[cmdIndex];
                maxConnections = atoi((const char*) cmd);
            }
            else if(arg == "-f")
            {
                formatDisk = true;
            }
            else if(arg == "--disk")
            {
                cmdIndex++;
                cmd = argv[cmdIndex];
                diskName = cmd;
            }
            else
                cout << arg << " is not a valid command." << endl;

            cmdIndex++;
            cmd = argv[cmdIndex];
        }

        // load directory and format disk if requested
        Disk disk = Disk(diskName, FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
        dirPtr = new FileDirectory(&disk, formatDisk);

        // create IPv4 TCP/IP socket
        sid = socket(AF_INET, SOCK_STREAM, 0);

        if(-1 == sid)
            cout << "Error: could not create socket. errno = " << errno << endl;
        else
            cout << "Socket created successfully." << endl;

        // bind socket to given port
        struct addrinfo hints;
        hints.ai_family = AF_INET;
        hints.ai_protocol = 0;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        hints.ai_addrlen = 0;
        hints.ai_addr = NULL;
        hints.ai_canonname = NULL;
        hints.ai_next = NULL;

        struct addrinfo* result;
        getaddrinfo(NULL, port, &hints, &result);

        if(-1 == bind(sid, result->ai_addr, result->ai_addrlen))
        {
            cout << "Error: failed in binding socket. errno = " << errno << endl;
            close(sid);
            return EXIT_FAILURE;
        }
        else
            cout << "Server bound to port " << port;

        struct sockaddr_in address;
        socklen_t addressLen = sizeof(address);
        getsockname(sid, (struct sockaddr *) &address, &addressLen);
        freeaddrinfo(result);

        int yes = 1;
        setsockopt(sid, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        // start listening for connections
        if(-1 == listen(sid, maxConnections))
        {
            cout << "Error: listening on socket failed. errno = " << errno << endl;
            close(sid);
            return EXIT_FAILURE;
        }
        else
            cout << ".\nServer listening for up to " << maxConnections << " connections." << endl;

        // connection acceptance loop
        struct sockaddr_in clientAddr;
        int maxThreads = maxConnections;
        pthread_t* threads = new pthread_t[maxThreads];
        connectionThreadParameter tParam[maxThreads];
        threadDone = new bool[maxThreads];
        for(int i = 0; i < maxThreads; i++)
            threadDone[i] = false;
        int nextThread = 0;
        int freeThreads = maxThreads;
        
        // establish thread to read user input
        pthread_t cinThread;
        int rtn_status = pthread_create(&cinThread, 0, inputThread, (void*) &disk);
        if (rtn_status)
        {
            cerr << "pthread creation failed: " << strerror(rtn_status) << endl;
            exit(-1);
        }

        while(true)
        {
            if(freeThreads > 0)
            {
                socklen_t len = (socklen_t) sizeof(struct sockaddr_in);
                tParam[nextThread].socID = accept(sid, (struct sockaddr *) &clientAddr, &len);
                if(tParam[nextThread].socID == -1)
                {
                    cout << "Error: failed to connect with a client.\n";
                    continue;
                }
                else
                    cout << "Client number " << (nextThread+1) << " connected." << endl;
                tParam[nextThread].threadNumber = nextThread;
                tParam[nextThread].dirPtr = dirPtr;
                tParam[nextThread].diskPtr = &disk;

                // create handler thread
                int rtn_status = pthread_create(&threads[nextThread], 0, connectionThread, (void*) &tParam[nextThread]);
                if (rtn_status)
                {
                    cerr << "pthread creation failed: " << strerror(rtn_status) << endl;
                    exit(-1);
                }

                threadDone[nextThread] = false;
                freeThreads--;
                nextThread++;
                for(int i = 0; i < maxThreads; i++)
                    if(threadDone[i])
                    {
                        pthread_join(threads[i], NULL);
                        nextThread = i;
                        freeThreads++;
                        break;
                    }
            }
            else
            {
                // max connections reached, wait for a client to disconnect
                cout << "Maximum number of connections reached. ";
                cout << "Waiting for a client to disconnect." << endl;
                pthread_cond_wait(&threadsFree, &cond_mutex);
                for(int i = 0; i < maxThreads; i++)
                    if(threadDone[i])
                    {
                        pthread_join(threads[i], NULL);
                        freeThreads++;
                        nextThread = i;
                    }
            }
        } // end while
    }
    else
    {
        // read in additional command-line arguments for client
        char* port = "4242";
        string hostname = "127.0.0.1";
        while(cmd != NULL)
        {
            arg = string(cmd);
            if(arg == "--port")
            {
                cmdIndex++;
                cmd = argv[cmdIndex];
                port = cmd;
            }
            else if(arg == "--server")
            {
                cmdIndex++;
                cmd = argv[cmdIndex];
                hostname = string(cmd);
            }
            else
                cout << arg << " is not a valid command." << endl;

            cmdIndex++;
            cmd = argv[cmdIndex];
        }

            // create IPv4 TCP/IP socket
            int sid = socket(AF_INET, SOCK_STREAM, 0);

            if(-1 == sid)
                cout << "Error: could not create socket. errno = " << errno << endl;
            else
                cout << "Socket created successfully." << endl;

            // connect to server
            struct addrinfo hints;
            hints.ai_family = AF_UNSPEC;
            hints.ai_protocol = 0;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = 0;
            hints.ai_addrlen = 0;
            hints.ai_addr = NULL;
            hints.ai_canonname = NULL;
            hints.ai_next = NULL;

            if(hostname == "localhost")
                hostname = "127.0.0.1";

            struct addrinfo* result;
            getaddrinfo(hostname.c_str(), (const char*) port, &hints, &result);

            if(-1 == connect(sid, result->ai_addr, result->ai_addrlen))
            {
                cout << "Error: could not connect to server. errno = " << errno << endl;
                close(sid);
                return EXIT_FAILURE;
            }
            else
                cout << "Connected to " << hostname << " on port " << port << endl;

            if(result != NULL)
                freeaddrinfo(result);

            // begin loop: read input from user, send to server
            int lineLen = 64;
            char line[lineLen];
            string lineStr = "";
            Message msg;
            RFSConnection con(sid);
            int fd = 0;
            bool fileOpen = false;
            bool displayMenu = true;            

            while(true)
            {
                if(displayMenu)
                {
                    cout << "\nCS5631 Lab 8 Client" << endl;
                    cout << "0) Exit Program" << endl;
                    cout << "1) Open a file" << endl;
                    cout << "2) Close a file" << endl;
                    cout << "3) List directory" << endl;
                    cout << "4) Delete file" << endl;
                    cout << "5) Check if file exists" << endl;
                    cout << "6) Write to file" << endl;
                    cout << "7) Read from file" << endl;
                    cout << "Enter Command: ";
                }
                displayMenu = true;

                // read user input
                cin.getline(line, lineLen);
                lineStr = string(line);

                if(lineStr == "0")
                {
                    con.quit();
                    close(sid);                    
                    return EXIT_SUCCESS;
                }
                else if (lineStr == "1")
                {
                    string f;
                    string m;
                    cout << "Filename: ";
                    cin >> f;
                    cin.ignore(); //ignore the \n
                    cout << "Open mode ('read' or 'write'): ";
                    cin >> m;
                    cin.ignore(); //ignore the \n


                    String filename = f.c_str();
                    String mode = m.c_str();
                    fd = con.openFile(filename, mode);
                    if(fd > -1)
                    {
                        cout << "OpenFile: fd = " << fd << endl;
                        fileOpen = true;
                    }
                    else
                        cout << "Error: OpenFile failed." << endl;
                }
                else if (lineStr == "2")
                {
                    cout << "File descriptor of file to close: ";
                    cin.getline(line, lineLen);
                    fd = atoi(line);
                    if(fileOpen)
                        if(-1 != con.closeFile(fd))
                        {
                            cout << "File closed successfully." << endl;
                            fileOpen = false;
                        }
                        else
                            cout << "Error: Failed to close file." << endl;
                    else
                        cout << "There is no file currently open." << endl;
                }
                else if (lineStr == "3")
                {
                    list<dirEntry> dirList = con.listDirectory();
                    cout << "Start of directory" << endl;
                    for(list<dirEntry>::iterator it = dirList.begin(); it != dirList.end(); it++)
                    {
                        cout << "Filename: " << it->filename;
                        cout << "; FCB number: " << it->fcbNum;
                        cout << "; Start block: " << it->startBlock;
                        cout << "; End block: " << it->endBlock;
                        cout << "; Number of blocks: " << it->numBlocks << endl;
                    }
                    cout << "End of directory" << endl;
                }
                else if (lineStr == "4")
                {
                    cout << "Name of file to delete: ";
                    string file;
                    cin >> file;
                    cin.ignore(); //ignore the \n

                    String filename = file.c_str();
                    if(-1 == con.deleteFile(filename))
                        cout << "File not found or could not be deleted." << endl;
                    else
                        cout << "File deleted successfully." << endl;
                }
                else if (lineStr == "5")
                {
                    cout << "Name of file: ";
                    string file;
                    cin >> file;
                    cin.ignore(); //ignore the \n

                    String filename = file.c_str();
                    int fe;
                    fe = con.fileExists(filename);

                    if(fe == 0)
                        cout << file << " does not exist." << endl;
                    else if(fe == 1)
                        cout << file << " exists." << endl;                    
                    else
                        cout << "Error: could not determine file's existence." << endl;
                }
                else if (lineStr == "6")
                {
                    cout << "File descriptor of file to write to: ";
                    cin.getline(line, lineLen);
                    fd = atoi(line);
                    cout << "Enter data to write to file: ";
                    cin.getline(line, lineLen);

                    int bytesWritten = con.writeFile(fd, string(line).length(), line);
                    if(-1 == bytesWritten)
                        cout << "Error: could not write to file." << endl;
                    else
                        cout << "Wrote " << bytesWritten << " bytes to file." << endl;
                }
                else if (lineStr == "7")
                {
                    cout << "File descriptor of file to read from: ";
                    cin.getline(line, lineLen);
                    fd = atoi(line);
                    cout << "Number of bytes to read from file: ";
                    int bytesToRead;
                    cin >> bytesToRead;
                    char readBuffer[bytesToRead];

                    int bytesRead = con.readFile(fd, bytesToRead, readBuffer);
                    if(0 > bytesRead)
                        cout << "Error: could not read from file." << endl;
                    else
                    {
                        char outputBuffer[bytesRead];
                        memcpy(outputBuffer, readBuffer, bytesRead);
                        cout << "Read " << bytesRead << " bytes: " << outputBuffer << endl;
                    }
                }
                else if (lineStr == "test1a" ) {
                    //need to lock until two or more clients connect
                    string f;
                    string m;
                    cout << "Filename: ";
                    cin >> f;
                    cin.ignore(); //ignore the \n
                    cout << "Open mode ('read' or 'write'): ";
                    cin >> m;
                    cin.ignore(); //ignore the \n


                    String filename = f.c_str();
                    String mode = m.c_str();
                    
                    //open file
                    fd = con.openFile(filename, mode);
                    if(fd > -1)
                    {
                        cout << "OpenFile: fd = " << fd << endl;
                        fileOpen = true;
                    }
                    else
                        cout << "Error: OpenFile failed." << endl;
                    
                    cout << "File descriptor of file to write to: ";
                    cin.getline(line, lineLen);
                    fd = atoi(line);
                    cout << "Enter data to write to file: ";
                    cin.getline(line, lineLen);

                    //write to file
                    usleep(1000);
                    int bytesWritten = con.writeFile(fd, string(line).length(), line);
                    if(-1 == bytesWritten)
                        cout << "Error: could not write to file." << endl;
                    else
                        cout << "Wrote " << bytesWritten << " bytes to file." << endl;
                    
                    //close file
                    if(fileOpen)
                        if(-1 != con.closeFile(fd))
                        {
                            cout << "File closed successfully." << endl;
                            fileOpen = false;
                        }
                        else
                            cout << "Error: Failed to close file." << endl;
                    else
                        cout << "There is no file currently open." << endl;
                    
                    //open file for reading
                    mode = "read";
                    cout << "Opening file " <<filename.Cstr() << endl;
                    fd = con.openFile(filename, mode);
                    if(fd > -1)
                    {
                        cout << "OpenFile: fd = " << fd << endl;
                        fileOpen = true;
                    }
                    else
                        cout << "Error: OpenFile failed." << endl;
                    
                    //read from file
                    int bytesToRead = string(line).length();
                    char readBuffer[bytesToRead];
                    
                    int bytesRead = con.readFile(fd, bytesToRead, readBuffer);
                    if(0 > bytesRead)
                        cout << "Error: could not read from file." << endl;
                    else
                    {
                        char outputBuffer[bytesRead];
                        memcpy(outputBuffer, readBuffer, bytesRead);
                        cout << "Read " << bytesRead << " bytes: " << outputBuffer << endl;
                    }
                }
                else if(lineStr == "")
                    displayMenu = false;
                else
                    cout << "Invalid command." << endl;
            }
    }
}

void* connectionThread(void* dataPtr)
{
    connectionThreadParameter* param = (connectionThreadParameter*) dataPtr;
    RFSConnection clientCon(param->socID, &gft, param->dirPtr, param->diskPtr);

    while(true)
    {
        int result = clientCon.handleRequest();
        cout << "Result = " <<result <<endl;
        // check for quit command
        if(1 == result)
        {
            close(param->socID);
            threadDone[param->threadNumber] = true;
            cout << "Client number " << param->threadNumber+1 << " disconnected." << endl;
            pthread_cond_signal(&threadsFree);
            pthread_exit(NULL);
        }
        else if(-1 == result)
            cout << "Error: failed in handling client request." << endl;

        // else, request was handled successfully
    }
}

void* inputThread(void* dataPtr)
{
    // initialize variables
    Disk* diskPtr = (Disk*) dataPtr;    
    string command = "";
    int startBlock = 0;
    int endBlock = 0;
    int numberOfBlocks = 0;
    bool done = false;

    while(!done)
    {
        // read user input
        cin >> command;

        // handle user input
        if(command == "quit")
        {
            done = true;
        }
        else if(command == "freeList")
        {
            // get freeList from disk
            FreeList freeList(diskPtr, false);

            // output free list
            freeList.output();
        }
        else if(command == "blockGroup")
        {
            // read block group information from user
            cin >> startBlock >> endBlock >> numberOfBlocks;
            
            // read block group from disk
            BlockGroup blkGroup(startBlock, endBlock, numberOfBlocks, NULL);

            // output block group
            blkGroup.output();
        }
        else
            cout << "Unrecognized server command." << endl;
    }

    // shutdown server
    dirPtr->flush();
    close(sid);
    exit(0);
}
