#include "system/SetupSystem.h"
#include "message/Message.h"
#include <iostream>
#include "headerFiles.h"
#include <pthread.h>

using namespace muscle;
using namespace std;

void* connectionThread(void* dataPtr);
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  threadsFree = PTHREAD_COND_INITIALIZER;
bool* threadDone;

struct threadParameter
{
    int socID;
    int threadNumber;
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
        threadParameter tParam[maxThreads];
        threadDone = new bool[maxThreads];
        for(int i = 0; i < maxThreads; i++)
            threadDone[i] = true;
        int nextThread = 0;
        int freeThreads = maxThreads;
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

                // create handler thread
                int rtn_status = pthread_create(&threads[nextThread], 0, connectionThread, (void*) &tParam[nextThread]);
                if (rtn_status)
                {
                    cerr << "pthread creation failed: " << strerror(rtn_status) << endl;
                    exit(-1);
                }

                threadDone[nextThread] = false;
                freeThreads--;
                for(int i = 0; i < maxThreads; i++)
                    if(threadDone[i])
                    {
                        nextThread = i;
                        break;
                    }
            }
            else
            {
                // join any threads that have completed
                for(int i = 0; i < maxThreads; i++)
                    if(threadDone[i])
                    {
                        pthread_join(threads[i], NULL);
                        freeThreads++;
                        nextThread = i;
                    }

                // return to accepting if any threads were joined
                if(freeThreads > 0)
                    continue;

                // wait for a client to disconnect
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
//                cout << "Waiting on client number " << (nextThread+1) << ".\n";
//                pthread_join(threads[nextThread], NULL);
//                cout << "Client number " << (nextThread+1) << " disconnected.\n";
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
            cout << "\nEnter message to server: (type 'quit' to exit)\n" << endl;

            while(true)
            {
                cout << "CS5631 Lab 6 Client" << endl;
                cout << "0) Exit Program" << endl;
                cout << "1) Open a file" << endl;
                cout << "2) Close a file" << endl;
                cout << "3) List directory" << endl;
                cout << "4) Delete file" << endl;
                cout << "Enter Command: ";

                // read user input
                cin.getline(line, lineLen);
                lineStr = string(line);

                if(listStr == "0")
                {
                    close(sid);
                    return EXIT_SUCCESS;
                }
                else if (lineStr == "1")
                {
                    cout << "Filename and open mode: (e.g. file1 read)";
                    string filename;
                    string mode;
                    cin >> filename >> mode;

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
                    if(fileOpen)
                        if(-1 != con.closeFile(fd))
                            cout << "File closed successfully." << endl;
                        else
                            cout << "Error: Failed to close file." << endl;
                    else
                        cout << "There is no file currently open.";
                }
                else if (lineStr == "3")
                {
                    cout << con.listDirectory() << endl;
                }
                else if (lineStr == "4")
                {
                    cout << "Name of file to delete: ";
                    string filename;
                    cin >> filename;
                    if(-1 == con.deleteFile(filename))
                        cout << "File not found or could not be deleted." << endl;
                    else
                        cout << "File deleted successfully." << endl;
                }
                else
                    cout << "Invalid command." << endl;
            }
    }
}

void* connectionThread(void* dataPtr)
{
    threadParameter* param = (threadParameter*) dataPtr;

    // test thread code
    int length = 64;
    char buffer[length];
    char* bPtr = buffer;
    string msgStr = "Message acknowledged.";
    const char* msg = msgStr.c_str();
    while(true)
    {
        if(0 > recv(param->socID, bPtr, length, 0))
            cout << "Error: failed in receiving from client. errno = " << errno << endl;
        else
        {
            cout << "Received message: " << bPtr << endl;

            // check for quit command
            if(string(bPtr) == "quit")
            {
                close(param->socID);
                threadDone[param->threadNumber] = true;
                cout << "Client number " << param->threadNumber+1 << " disconnected." << endl;
                pthread_cond_signal(&threadsFree);
                pthread_exit(NULL);
            }

            if(0 > send(param->socID, msg, msgStr.length(), 0))
                cout << "Error: failed in sending to client. errno = " << errno << endl;
            else
                cout << "Sent acknowledgement of message.\n" << endl ;
        }
    }
}