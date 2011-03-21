/* 
 * File:   main.cpp
 * Author: James Ekstrom, Kaleb Jungclaus, Matt Fischer
 *
 * Created on February 4, 2011, 4:24 PM
 */
#include <cstdlib>

#include "FileSystemHeaders.h"
#include <iostream>
#include <sstream>
#include <string.h>
#include <fstream>


using namespace std;

int lab1Test();
int lab2Test();
int lab3Test();
//int search(vector<File>, string);

/*
 * Performs testing
 */
int main() {
    lab3Test();
    return EXIT_SUCCESS;
}

int lab1Test() {
    int command;

    BlockGroup* bgPtr;
    FreeList* flPtr;
    while (true) {
        cout << "CS5631 Team Lab 1 Tester" << endl;
        cout << "0) Exit Program" << endl;
        cout << "1) Create and format FreeList" << endl;
        cout << "2) Open and don't format FreeList" << endl;
        cout << "3) Close FreeList" << endl;
        cout << "4) Output block numbers in FreeList" << endl;
        cout << "5) Create BlockGroup" << endl;
        cout << "6) Return BlockGroup to FreeList" << endl;
        cout << "7) Add block to BlockGroup" << endl;
        cout << "8) Output block numbers of BlockGroup" << endl;
        cout << "9) Re-open BlockGroup" << endl;
        cout << "Enter Command: ";
        cin >> command;

        if (cin.peek() == '\n' && cin.good()) {
            switch (command) {
                case 0:
                    return (EXIT_SUCCESS);
                    break;
                case 1:
                    flPtr = new FreeList("testDisk", true);
                    cout << "Free list created.\n";
                    break;
                case 2:
                    flPtr = new FreeList("testDisk", false);
                    cout << "Free list opened.\n";
                    break;
                case 3:
                    if (flPtr != NULL) {
                        if (flPtr->close()) {
                            flPtr = NULL;
                            cout << "Free List closed.\n";
                        }
                    } else
                        cout << "There is no free list open.\n";
                    break;
                case 4:
                    if (flPtr != NULL)
                        flPtr->output();
                    else
                        cout << "There is no free list open.\n";
                    break;
                case 5:
                    if (flPtr != NULL) {
                        bgPtr = flPtr->createNew();
                        cout << "New block group created.\n";
                    } else
                        cout << "No free list to create block group from.\n";
                    break;
                case 6:
                    if (flPtr != NULL)
                        if (bgPtr != NULL) {
                            flPtr->returnBlocks(bgPtr);
                            delete bgPtr;
                            bgPtr = NULL;
                            cout << "Blocks returned.";
                        } else
                            cout << "No block group to return.\n";
                    else
                        cout << "There is no free list open.\n";
                    break;
                case 7:
                    if (bgPtr != NULL)
                        if (bgPtr->addBlock())
                            cout << "Block added.\n";
                        else
                            cout << "There is no block group.\n";
                    break;
                case 8:
                    if (bgPtr != NULL)
                        bgPtr->output();
                    else
                        cout << "There is no block group.\n";
                    break;
                case 9:
                    if (flPtr != NULL) {
                        int start;
                        int end;
                        int len;
                        cout << "Starting block Number? ";
                        cin >> start;
                        cout << "Ending block number? ";
                        cin >> end;
                        cout << "Number of blocks? ";
                        cin >> len;
                        bgPtr = new BlockGroup(start, end, len, flPtr);
                        cout << "Block group opened.\n";
                    } else
                        cout << "There is no free list open.\n";
                    break;
                default:
                    cout << "Bad input" << endl;
                    break;
            }
        } else {
            cout << "Bad input" << endl;

        }

        cout << "\n\n";
    }
}

int lab2Test() {
    int command = -1;
    bool showMenu = true;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);

    Directory* dirPtr = NULL;
    bool dirOpen = false;

    while (true) {
        cout << "CS5631 Team Lab 2 Tester" << endl;
        if (showMenu) {
            cout << "0) Exit Program" << endl;
            cout << "1) Create and format new Directory" << endl;
            cout << "2) Open existing Directory" << endl;
            cout << "3) Flush Directory to disk" << endl;
            cout << "4) Add an Entry to the Directory" << endl;
            cout << "5) Find an Entry in the Directory" << endl;
            cout << "6) Rename an Entry in the Directory" << endl;
            cout << "7) Remove an Entry from the Directory" << endl;
            cout << "8) Display all Entries in the Directory" << endl;
            cout << "9) Add 14 Entries (the number of entries in one disk block)" << endl;
            showMenu = false;
        } else {
            cout << "..." << endl;
        }
        cout << "Enter Command: ";
        cin >> command;

        if (cin.peek() == '\n' && cin.good()) {
            switch (command) {
                case 0:
                    return (EXIT_SUCCESS);
                    break;
                case 1:
                    if (!dirOpen) {
                        dirPtr = new Directory(&testDisk, true);
                        dirOpen = true;
                        cout << "Directory created.\n";
                    } else
                        cout << "There is already a directory open.\n";
                    break;
                case 2:
                    if (!dirOpen) {
                        dirPtr = new Directory(&testDisk, false);
                        dirOpen = true;
                        cout << "Directory opened.\n";
                    } else
                        cout << "There is already a directory open.\n";
                    break;
                case 3:
                    if (dirOpen) {
                        if (dirPtr->flush())
                            cout << "Directory flush successful.\n";
                        else
                            cout << "Directory flush failed.\n";
                    } else
                        cout << "There is no directory open.\n";
                    break;
                case 4:
                    if (dirOpen) {
                        string name;
                        int fcb;

                        cout << "Enter a name for the new Entry: ";
                        cin >> name;
                        cout << "Enter an FCB number for the new Entry: ";
                        cin >> fcb;

                        if (dirPtr->addFile(name, fcb))
                            cout << "Entry added successfully.\n";
                        else
                            cout << "Failed in adding Entry.\n";
                    } else
                        cout << "There is no directory open.\n";
                    break;
                case 5:
                    if (dirOpen) {
                        string name;
                        int fcb;

                        cout << "Enter a name to search for: ";
                        cin >> name;

                        fcb = dirPtr->findFile(name);
                        if (-1 == fcb)
                            cout << "Entry not found.\n";
                        else
                            cout << "Entry found, contains FCB number " << fcb << ".\n";
                    } else
                        cout << "There is no directory open.\n";
                    break;
                case 6:
                    if (dirOpen) {
                        string oldName;
                        string newName;

                        cout << "Enter the name you wish to replace: ";
                        cin >> oldName;
                        cout << "Enter a new name for the Entry: ";
                        cin >> newName;

                        if (dirPtr->renameFile(oldName, newName))
                            cout << "Entry found and successfully renamed.\n";
                        else
                            cout << "Entry not found.\n";
                    } else
                        cout << "There is no directory open.\n";
                    break;
                case 7:
                    if (dirOpen) {
                        string name;

                        cout << "Enter the name of the Entry to remove: ";
                        cin >> name;

                        if (dirPtr->removeFile(name))
                            cout << "Entry removed successfully.\n";
                        else
                            cout << "Entry not found or couldn't be removed.\n";
                    } else
                        cout << "There is no directory open.\n";
                    break;
                case 8:
                    if (dirOpen) {
                        list<Entry> entries = dirPtr->listEntries();

                        cout << "Start of Directory\n";
                        for (list<Entry>::iterator it = entries.begin(); it != entries.end(); it++)
                            cout << "Name: " << (*it).name << "; FCB number: " << (*it).fcb << "\n";
                        cout << "End of Directory\n";
                    } else
                        cout << "There is no directory open.\n";
                    break;
                case 9:
                    if (dirOpen) {
                        for (int i = 1; i < 15; i++) {
                            stringstream nameStream("");
                            nameStream << "entry" << i;
                            if (dirPtr->addFile(nameStream.str(), i))
                                cout << "Entry " << i << " added successfully.\n";
                            else
                                cout << "Failed in adding Entry " << i << ".\n";
                        }
                    } else
                        cout << "There is no directory open.\n";
                    break;
                default:
                    cout << "Bad input\n" << endl;
                    break;
            }
        } else {
            cout << "Bad inpu\nt" << endl;

        }

        cout << "\n";
    }
    delete dirPtr;
}

int lab3Test() {
    ofstream outFile;
    int command = -1;
    bool showMenu = true;
    list<File> files;
    list<File>::iterator i;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    File *f;
    Directory* dirPtr = new Directory(&testDisk, true);
    string name, writeData;
    list<Entry> entries;

    int mode, result, writeResult, bytesToRead, readResult;
    bool creat = false;
    bool readAccess = false;

    outFile.open("test.txt");
    while (true) {
        cout << "CS5631 Team Lab 3 Tester" << endl;
        if (showMenu) {
            cout << "0) Exit Program" << endl;
            outFile << "0) Exit Program" << endl;
            cout << "1) Create a file" << endl;
            outFile << "1) Create a file" << endl;
            cout << "2) Open an existing file" << endl;
            outFile << "2) Open an existing file" << endl;
            cout << "3) Close an existing file" << endl;
            outFile << "3) Close an existing file" << endl;
            cout << "4) Write to file" << endl;
            outFile << "4) Write to file" << endl;
            cout << "5) Read from file" << endl;
            outFile << "5) Read from file" << endl;
            cout << "6) Display all Entries in the Directory" << endl;
            outFile << "6) Display all Entries in the Directory" << endl;
            showMenu = false;
        } else {
            cout << "..." << endl;
            outFile << "..." << endl;
        }
        cout << "Enter Command: ";
        cin >> command;

        if (cin.peek() == '\n' && cin.good()) {
            switch (command) {
                case 0:
                    outFile.close();
                    return (EXIT_SUCCESS);
                    break;

                case 1: //create a file

                    cout << "Enter a name for the new file: ";
                    cin >> name;
                    outFile << "Enter a name for the new file: " << name << endl;

                    //file is created and added to directory in file ctor
                    creat = true;
                    readAccess = false;
                    f = new File(name, creat, readAccess, &testDisk, dirPtr);
                    files.push_back(*f);

                    //maybe have addfile(File) ???

                    //make sure file is in directory
                    if (dirPtr->findFile(name) != -1) {
                        cout << endl << "Succesfully created file" << endl;
                        outFile << "\nSuccessfully created file" << endl;
                        //write changes to disk ??
                        dirPtr->flush();
                    } else {
                        cout << endl << "File already exists" << endl;
                        outFile << "\nFile already exists" << endl;
                    }

                    delete f;
                    break;

                case 2: //open an existing file

                    cout << "Enter name of file: ";
                    cin >> name;
                    outFile << "Enter name of file: " << name << endl;
                    cout << endl << "Enter mode (0 = write, 1 = read): ";
                    cin >> mode;
                    outFile << "Enter mode (0 = write, 1 = read): " << mode << endl;

                    creat = false;
                    readAccess = mode;

                    if (mode > 1 || mode < 0) {
                        cout << endl << "Invalid mode" << endl;
                        outFile << "\nInvalid mode" << endl;
                        break;
                    }
                    result = -1;
                    //result = search(files, name);
                    for (i = files.begin(); i != files.end(); i++) {
                        if (strcmp(i->getName().c_str(), name.c_str()) == 0) {
                            result = 1;
                            break;
                        } else
                            result = -1;
                    }

                    if (result != -1) {
                        if (i->open(mode))
                            break;
                        else {
                            cout << "File already opened" << endl;
                            outFile << "File already opened\n";
                        }
                    } else {
                        try {
                            f = new File(name, creat, readAccess, &testDisk, dirPtr);
                            files.push_back(*f);
                        } catch (FileNotFoundException e) {
                            cout << endl << "File not opened" << endl;
                            outFile << "\nFile not opened" << endl;
                            break;
                        }


                    }

                    cout << endl << "File opened" << endl;
                    outFile << "\nFile opened" << endl;
                    delete f;
                    break;

                case 3: //close an existing file
                    cout << "Enter name of file: ";
                    cin >> name;
                    outFile << "Enter name of file: " << name << endl;
                    //result = search(files, name);
                    result = -1;
                    for (i = files.begin(); i != files.end(); i++) {
                        if (strcmp(i->getName().c_str(), name.c_str()) == 0) {
                            result = 1;
                            break;
                        } else
                            result = -1;
                    }

                    if (result != -1) {
                        i->close();
                        cout << endl << "Successfully closed file" << endl;
                        outFile << "\nSuccessfully closed file" << endl;
                        dirPtr->flush();
                        break;
                    } else {
                        cout << endl << "File not found" << endl;
                        outFile << "\nFile not found" << endl;
                        break;
                    }

                case 4: //write to file
                    cout << "Enter name of file: ";
                    cin >> name;
                    outFile << "Enter name of file: " << name << endl;
                    //result = search(files, name);
                    for (i = files.begin(); i != files.end(); i++) {
                        if (strcmp(i->getName().c_str(), name.c_str()) == 0) {
                            result = 1;
                            break;
                        } else
                            result = -1;
                    }
                    if (result != -1) {
                        if (i->isOpenForRead()) {
                            cout << endl << "File open for reading" << endl;
                            outFile << "\nFile open for reading" << endl;
                            break;
                        } else {
                            cout << endl << "Enter data: ";
                            cin >> writeData;
                            outFile << "\nEnter data: " << writeData << endl;
                            char *temp = new char[writeData.size()];
                            for (int i = 0; i < writeData.size(); i++)
                                temp[i] = writeData.c_str()[i];
                            writeResult = i->write(temp,
                                    writeData.size());

                            if (writeResult != -1) {
                                cout << endl << writeResult <<
                                        " bytes of data written successfully" << endl;
                                outFile << "\n" << writeResult << " bytes of data"
                                        "successfully written" << endl;
                                break;
                            } else {
                                cout << endl << "Data written unsuccessfully" << endl;
                                outFile << "\nData written unsuccessfully" << endl;
                                break;
                            }
                        }
                    } else {
                        cout << endl << "File not found" << endl;
                        outFile << "\nFile not found" << endl;
                        break;
                    }

                case 5: //read from file
                    cout << "Enter name of file: ";
                    cin >> name;
                    outFile << "enter name of file: " << name << endl;
                    //result = search(files, name);
                    for (i = files.begin(); i != files.end(); i++) {
                        if (strcmp(i->getName().c_str(), name.c_str()) == 0) {
                            result = 1;
                            break;
                        } else
                            result = -1;
                    }

                    if (result != -1) {
                        if (!i->isOpenForRead()) {
                            cout << endl << "File open for writing" << endl;
                            outFile << "\nFile open for writing" << endl;
                            break;
                        } else {
                            cout << endl << "Bytes to read: ";
                            cin >> bytesToRead;
                            outFile << "\nBytes to read: " << bytesToRead << endl;

                            char* readBuffer = new char[bytesToRead];
                            for (int i = 0; i < bytesToRead; i++)
                                readBuffer[i] = 0;


                            readResult = i->read(readBuffer,
                                    bytesToRead);

                            if (readResult != -1) {
                                cout << endl << readResult <<
                                        " bytes of data read successfully" << endl;

                                outFile << "\n" << readResult << " bytes of data "
                                        "read successfully" << endl;

                                cout << "Data read: " << readBuffer << endl;
                                outFile << "Data read: " << readBuffer << endl;
                                break;
                            } else {
                                cout << endl << "Data read unsuccessfully" << endl;
                                outFile << "\nData read unsuccessfully" << endl;
                                break;
                            }
                        }
                    } else {
                        cout << endl << "File not found" << endl;
                        outFile << "\nFile not found" << endl;
                        break;
                    }

                case 6: //dsplay all entries in the directory
                    entries = dirPtr->listEntries();

                    cout << "Start of Directory\n";
                    outFile << "Start of Directory\n";
                    for (list<Entry>::iterator it = entries.begin(); it != entries.end(); it++) {
                        cout << "Name: " << (*it).name << "; FCB number: " << (*it).fcb << "\n";
                        outFile << "Name: " << (*it).name << "; FCB number: " << (*it).fcb << "\n";
                    }
                    cout << "End of Directory\n";
                    outFile << "End of Directory\n";
                    break;

                default:
                    cout << "Bad input\n" << endl;
                    outFile << "Bad input\n" << endl;
                    break;
            }
        } else {
            cout << "Bad input\n" << endl;
            outFile << "Bad input\n";

        }

        cout << "\n";
    }
    delete dirPtr;
}

/**
 * Searches any recently created files (in memory) that can be opened for read
 * or write access (open file table)
 * @param files the vector containing any open files
 * @param name the name of the file to search for
 * @return -1 if file is not found, else the index of the file in the vector
 */
//int search(list<File> files, string name) {
//    list<File>::iterator i;
//    for (i = files.begin(); i != files.end(); i++) {
//        if (strcmp(i->getName().c_str(), name.c_str()) == 0) {
//            break;
//        } else
//            i = 0;
//    }
//    return i;
//}
