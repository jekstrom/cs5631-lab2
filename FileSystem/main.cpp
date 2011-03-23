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
#include <list>


using namespace std;

int lab1Test();
int lab2Test();
int lab3Test();
void lab3test1();
void lab3test2a();
void lab3test2b();
void lab3test3();
void lab3test4a();
void lab3test4b();
void createSource();
void lab3test5a();
void lab3test5b();
void lab3test6();
void lab3test7Before();
void lab3test7After();
void lab3test8();
void lab3test9();
void lab3test10a();
void lab3test10b();
void lab3test10c();
void lab3test11();


//int search(vector<File>, string);

/*
 * Performs testing
 */
int main() {
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    ofstream outFile;
    outFile.open("main.txt");
    int command = -1;
    while (true) {
        cout << "CS5631 Team Lab 3 Tester" << endl;
        if (true) {
            cout << "0) Exit Program" << endl;
            outFile << "0) Exit Program" << endl;
            cout << "1) Run test 1" << endl;
            outFile << "1) Run test 1" << endl;
            cout << "2) Run test 2a" << endl;
            outFile << "2) Run test 2a" << endl;
            cout << "3) Run test 2b" << endl;
            outFile << "3) Run test 2b" << endl;
            cout << "4) Run test 3" << endl;
            outFile << "4) Run test 3" << endl;
            cout << "5) Run test 4a (before Shutdown)" << endl;
            outFile << "5) Run test 4a (Before Shutdown)" << endl;
            cout << "6) Run test 4b (after Shutdown)" << endl;
            outFile << "6) Run test 4b (after Shutdown)" << endl;
            cout << "7) Create SOURCE" << endl;
            outFile << "7) Create SOURCE" << endl;
            cout << "8) Run test 5a" << endl;
            outFile << "8) Run test 5a" << endl;
            cout << "9) Run test 5b" << endl;
            outFile << "9) Run test 5b" << endl;
            cout << "10) Run test 6" << endl;
            outFile << "10) Run test 6" << endl;
            cout << "11) Run test 11" << endl;
            outFile << "11) Run test 11" << endl;
            cout << "12) Run test 7 Before" <<endl;
            outFile << "12) Run test 7 Before" <<endl;
            cout << "13) Run test 7 After" <<endl;
            outFile << "13) Run test 7 After" <<endl;
            cout << "14) Run test 8" <<endl;
            outFile << "14) Run test 8" <<endl;
            cout << "15) Run test 9" <<endl;
            outFile << "15) Run test 9" <<endl;
            cout << "16) Run test 10a" <<endl;
            outFile << "16) Run test 10a" <<endl;
            cout << "17) Run test 10b" <<endl;
            outFile << "17) Run test 10b" <<endl;
            cout << "18) Run test 10c" <<endl;
            outFile << "18) Run test 10c" <<endl;
        } else {
            cout << "..." << endl;
            outFile << "..." << endl;
        }
        cout << "Enter Command: ";
        cin >> command;

        if (cin.peek() == '\n' && cin.good()) {
            switch (command) {
                case 0: //exit
                    return (EXIT_SUCCESS);
                    break;

                case 1: //run test 1
                    lab3test1();
                    break;

                case 2: //run test 2a
                    lab3test2a();
                    break;

                case 3: //run test 2b
                    lab3test2b();
                    break;

                case 4: //run test 3
                    lab3test3();
                    break;

                case 5: //run test 4a
                    lab3test4a();
                    break;

                case 6: //run test 4b
                    lab3test4b();
                    break;

                case 7: //run test createSource
                    createSource();
                    break;

                case 8: //run test 5a
                    lab3test5a();
                    break;

                case 9: //run test 5b
                    lab3test5b();
                    break;

                case 10: //run test 6
                    lab3test6();
                    break;

                case 11:
                    lab3test11();
                    break;

                case 12:
                    lab3test7Before();
                    break;

                case 13:
                    lab3test7After();
                    break;

                case 14:
                    lab3test8();
                    break;

                case 15:
                    lab3test9();
                    break;

                case 16:
                    lab3test10a();
                    break;

                case 17:
                    lab3test10b();
                    break;

                case 18:
                    lab3test10c();
                    break;

                default:
                    cout << "bad input";
                    break;

            }

        }
        return EXIT_SUCCESS;
    }
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

                case 6: //display all entries in the directory
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

void lab3test1() {
    ofstream outFile;
    outFile.open("test1.txt");

    bool creat;
    bool readAccess;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, true);

    //create a new file, open it for reading
    cout << "Creating file test1" << endl;
    outFile << "Creating file test1" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f = new File("test1", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("test1") != -1) {
        cout << endl << "Succesfully created file" << endl;
        outFile << "\nSuccessfully created file" << endl;
        //write changes to disk ??
        dirPtr->flush();
    } else {
        cout << endl << "File already exists" << endl;
        outFile << "\nFile already exists" << endl;
    }

    string writeData = "0123456789";
    outFile << "Data being written to file: " << writeData << endl;
    int writeResult = -1;
    char *temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    writeResult = f->write(temp,
            writeData.size());

    if (writeResult != -1) {
        cout << endl << writeResult <<
                " bytes of data written successfully" << endl;
        outFile << "\n" << writeResult << " bytes of data"
                "successfully written" << endl;
    } else {
        cout << endl << "Data written unsuccessfully" << endl;
        outFile << "\nData written unsuccessfully" << endl;
    }

    //close file
    f->close();

    //open file for reading, read data
    f->open(true);

    int bytesToRead = 10;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    char readBuffer[bytesToRead + 1];
    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;


    int readResult = f->read(readBuffer,
            bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully" << endl;
        outFile << "\nData read unsuccessfully" << endl;
    }

    f->close();
    f->open(true);
    //try to read past end of file
    //file only has 10 bytes
    bytesToRead = 20;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    readBuffer[bytesToRead + 1];
    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;


    readResult = f->read(readBuffer,
            bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully" << endl;
        outFile << "\nData read unsuccessfully" << endl;
    }

    delete f;
}

void lab3test2a() {
    ofstream outFile;
    outFile.open("test2a.txt");

    bool creat;
    bool readAccess;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, true);

    //create a new file, open it for writing
    cout << "Creating file test2" << endl;
    outFile << "Creating file test2" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f = new File("test2", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("test2") != -1) {
        cout << endl << "Succesfully created file" << endl;
        outFile << "\nSuccessfully created file" << endl;
        //write changes to disk
        dirPtr->flush();
    } else {
        cout << endl << "File already exists" << endl;
        outFile << "\nFile already exists" << endl;
    }

    //writeData = 100 bytes
    string writeData = "012345678901234567890123456789012345678901234567890123456789"
            "0123456789012345678901234567890123456789";
    outFile << "Data being written to file(x6): " << writeData << endl;
    int writeResult = -1;
    char *temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    int totalBytes = 0;
    for (int i = 0; i < 6; i++) { //write 600 bytes to disk
        writeResult = f->write(temp, writeData.size());

        if (writeResult != -1) {
            totalBytes += writeResult;
            cout << endl << writeResult <<
                    " bytes of data written successfully" << endl;
            outFile << "\n" << writeResult << " bytes of data"
                    "successfully written" << endl;
            cout << endl << totalBytes <<
                    " total bytes of data written successfully" << endl;
            outFile << "\n" << totalBytes << " total bytes of data"
                    "successfully written" << endl;
        } else {
            cout << endl << "Data written unsuccessfully" << endl;
            outFile << "\nData written unsuccessfully" << endl;
        }

    }

    //close file
    f->close();

    //open file for reading, read data
    f->open(true);

    //read 100 bytes 6 times
    int bytesToRead = 100;

    for (int i = 0; i < 6; i++) {
        cout << endl << "Bytes to read: " << bytesToRead;
        outFile << "\nBytes to read: " << bytesToRead << endl;

        char readBuffer[bytesToRead + 1];
        for (int i = 0; i < bytesToRead + 1; i++)
            readBuffer[i] = 0;


        int readResult = f->read(readBuffer, bytesToRead);

        if (readResult != -1) {
            cout << endl << readResult <<
                    " bytes of data read successfully" << endl;

            outFile << "\n" << readResult << " bytes of data "
                    "read successfully" << endl;

            cout << "Data read: " << readBuffer << endl;
            outFile << "Data read: " << readBuffer << endl;
        } else {
            cout << endl << "Data read unsuccessfully" << endl;
            outFile << "\nData read unsuccessfully" << endl;
        }
    }

    f->close();
    f->open(true);

    cout << "----------------------------" << endl;
    cout << "Reading 7 times (700 bytes)" << endl;
    outFile << "----------------------------" << endl;
    outFile << "Reading 7 times (700 bytes)" << endl;

    //try to read past end of file
    //read 100 bytes 7 times, going past the 600 bytes
    bytesToRead = 100;
    for (int i = 0; i < 7; i++) {
        if (i >= 6) {
            cout << "\nTrying to read past EOF" << endl;
            outFile << "\nTrying to read past EOF" << endl;
        }
        cout << endl << "Bytes to read: " << bytesToRead;
        outFile << "\nBytes to read: " << bytesToRead << endl;

        char readBuffer[bytesToRead + 1];
        for (int i = 0; i < bytesToRead + 1; i++)
            readBuffer[i] = 0;


        int readResult = f->read(readBuffer, bytesToRead);

        if (readResult != -1) {
            cout << endl << readResult <<
                    " bytes of data read successfully" << endl;

            outFile << "\n" << readResult << " bytes of data "
                    "read successfully" << endl;

            cout << "Data read: " << readBuffer << endl;
            outFile << "Data read: " << readBuffer << endl;
        } else {
            cout << endl << "Data read unsuccessfully" << endl;
            outFile << "\nData read unsuccessfully" << endl;
        }
    }

    delete f;
}

void lab3test2b() {
    ofstream outFile;
    outFile.open("test2b.txt");

    bool creat;
    bool readAccess;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, true);

    //create a new file, open it for writing
    cout << "Creating file test2b" << endl;
    outFile << "Creating file test2b" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f = new File("test2b", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("test2b") != -1) {
        cout << endl << "Succesfully created file" << endl;
        outFile << "\nSuccessfully created file" << endl;
        //write changes to disk
        dirPtr->flush();
    } else {
        cout << endl << "File already exists" << endl;
        outFile << "\nFile already exists" << endl;
    }

    //writeData = 600 byes
    string writeData = "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789";
    outFile << "Data being written to file(x6): " << writeData << endl;
    int writeResult = -1;
    char *temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    writeResult = f->write(temp, writeData.size());

    if (writeResult != -1) {
        cout << endl << writeResult <<
                " bytes of data written successfully" << endl;
        outFile << "\n" << writeResult << " bytes of data"
                " successfully written" << endl;
    } else {
        cout << endl << "Data written unsuccessfully" << endl;
        outFile << "\nData written unsuccessfully" << endl;
    }

    //close file
    f->close();

    //open file for reading, read data
    f->open(true);

    //read 600 bytes
    int bytesToRead = 600;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    char readBuffer[bytesToRead + 1];
    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = '\0';


    int readResult = f->read(readBuffer, bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully" << endl;
        outFile << "\nData read unsuccessfully" << endl;
    }

    f->close();
    f->open(true);
    //try to read past end of file
    //read 700 bytes, going past the 600 bytes
    bytesToRead = 650;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    readBuffer[bytesToRead + 1];
    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;


    readResult = f->read(readBuffer, bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully" << endl;
        outFile << "\nData read unsuccessfully" << endl;
    }

    delete f;
}

void lab3test3() {
    ofstream outFile;
    outFile.open("test3.txt");

    bool creat;
    bool readAccess;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, true);

    //create a new file, open it for writing
    cout << "Creating file test3" << endl;
    outFile << "Creating file test3" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f = new File("test3", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("test3") != -1) {
        cout << endl << "Succesfully created file" << endl;
        outFile << "\nSuccessfully created file" << endl;
        //write changes to disk
        dirPtr->flush();
    } else {
        cout << endl << "File already exists" << endl;
        outFile << "\nFile already exists" << endl;
    }

    //writeData = 1 byte 100 times
    for (int i = 0; i < 100; i++) {
        char writeData[2] = {0};
        if (i % 10 < 5)
            writeData[0] = 'a';
        else
            writeData[0] = 'b';
        outFile << "Data being written to file: " << *writeData << endl;
        int writeResult = -1;
        int totalBytes = 0;
        writeResult = f->write(writeData, 1);

        if (writeResult != -1) {
            cout << endl << writeResult <<
                    " bytes of data written successfully" << endl;
            outFile << "\n" << writeResult << " bytes of data"
                    "successfully written" << endl;
            cout << endl << totalBytes <<
                    " total bytes of data written successfully" << endl;
            outFile << "\n" << totalBytes << " total bytes of data"
                    "successfully written" << endl;
        } else {
            cout << endl << "Data written unsuccessfully" << endl;
            outFile << "\nData written unsuccessfully" << endl;
        }
        totalBytes += writeResult;
    }

    //close file
    f->close();

    //open file for reading, read data
    f->open(true);

    //read 1 byte 100 times
    int bytesToRead = 1;
    for (int i = 0; i < 100; i++) {
        cout << endl << "Bytes to read: " << bytesToRead;
        outFile << "\nBytes to read: " << bytesToRead << endl;

        char readBuffer[bytesToRead + 1];
        for (int i = 0; i < bytesToRead + 1; i++)
            readBuffer[i] = 0;

        int readResult = f->read(readBuffer, bytesToRead);

        if (readResult != -1) {
            cout << endl << readResult <<
                    " bytes of data read successfully" << endl;

            outFile << "\n" << readResult << " bytes of data "
                    "read successfully" << endl;

            cout << "Data read: " << readBuffer << endl;
            outFile << "Data read: " << readBuffer << endl;
        } else {
            cout << endl << "Data read unsuccessfully" << endl;
            outFile << "\nData read unsuccessfully" << endl;
        }
    }

    f->close();
    f->open(true);
    //try to read past end of file
    //read 1 byte 101 times, going past the 100 bytes
    bytesToRead = 1;
    for (int i = 0; i < 101; i++) {
        cout << endl << "Bytes to read: " << bytesToRead;
        outFile << "\nBytes to read: " << bytesToRead << endl;

        char readBuffer[bytesToRead + 1];
        for (int i = 0; i < bytesToRead + 1; i++)
            readBuffer[i] = 0;


        int readResult = f->read(readBuffer,
                bytesToRead);

        if (readResult != -1) {
            cout << endl << readResult <<
                    " bytes of data read successfully" << endl;

            outFile << "\n" << readResult << " bytes of data "
                    "read successfully" << endl;

            cout << "Data read: " << readBuffer << endl;
            outFile << "Data read: " << readBuffer << endl;
        } else {
            cout << endl << "Data read unsuccessfully" << endl;
            outFile << "\nData read unsuccessfully" << endl;
        }
    }

    delete f;
}

void lab3test4a() {
    ofstream outFile;
    outFile.open("test4a.txt");

    bool creat;
    bool readAccess;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, true);

    //create a new file, open it for writing
    cout << "Creating file test4" << endl;
    outFile << "Creating file test4" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f = new File("test4", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("test4") != -1) {
        cout << endl << "Succesfully created file" << endl;
        outFile << "\nSuccessfully created file" << endl;
        //write changes to disk
        dirPtr->flush();
    } else {
        cout << endl << "File already exists" << endl;
        outFile << "\nFile already exists" << endl;
    }

    //writeData = 100 byes
    string writeData = "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789";
    outFile << "Data being written to file(x6): " << writeData << endl;
    int writeResult = -1;
    char *temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    int totalBytes = 0;
    for (int i = 0; i < 6; i++) { //write 600 bytes to disk
        writeResult = f->write(temp, writeData.size());

        if (writeResult != -1) {
            cout << endl << writeResult <<
                    " bytes of data written successfully" << endl;
            outFile << "\n" << writeResult << " bytes of data"
                    "successfully written" << endl;
            cout << endl << totalBytes <<
                    " total bytes of data written successfully" << endl;
            outFile << "\n" << totalBytes << " total bytes of data"
                    "successfully written" << endl;
        } else {
            cout << endl << "Data written unsuccessfully" << endl;
            outFile << "\nData written unsuccessfully" << endl;
        }
        totalBytes += writeResult;
    }

    //close file
    f->close();

    delete dirPtr;
    delete f;
    cout << "Shutting down program..." << endl;
    outFile << "Shutting down program..." << endl;
    exit(1);
}

void lab3test4b() {
    ofstream outFile;
    outFile.open("test4b.txt");
    bool creat = false;
    bool readAccess = true;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, creat);

    //after restarting program...
    cout << "Program starting back up..." << endl;
    outFile << "Program starting back up..." << endl;
    cout << "Opening file test4 for read access" << endl;
    outFile << "Opening file test4 for read access" << endl;

    File *f;
    try { //open file for reading access
        f = new File("test4", creat, readAccess, &testDisk, dirPtr);
    } catch (FileNotFoundException e) {
        cout << endl << "File not opened" << endl;
        outFile << "\nFile not opened" << endl;
    }

    int bytesToRead = 100;
    for (int i = 0; i < 6; i++) {
        cout << endl << "Bytes to read: " << bytesToRead;
        outFile << "\nBytes to read: " << bytesToRead << endl;

        char readBuffer[bytesToRead + 1];
        for (int i = 0; i < bytesToRead + 1; i++)
            readBuffer[i] = 0;


        int readResult = f->read(readBuffer,
                bytesToRead);

        if (readResult != -1) {
            cout << endl << readResult <<
                    " bytes of data read successfully" << endl;

            outFile << "\n" << readResult << " bytes of data "
                    "read successfully" << endl;

            cout << "Data read: " << readBuffer << endl;
            outFile << "Data read: " << readBuffer << endl;
        } else {
            cout << endl << "Data read unsuccessfully" << endl;
            outFile << "\nData read unsuccessfully" << endl;
        }
    }

    delete f;
    delete dirPtr;
}

void createSource() {
    //make file that contains 10,000 bytes of data
    ofstream outFile;
    outFile.open("test5aSource.txt");

    bool creat;
    bool readAccess;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, true);

    //create a new file, open it for writing
    cout << "Creating file SOURCE" << endl;
    outFile << "Creating file SOURCE" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f = new File("SOURCE", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("SOURCE") != -1) {
        cout << endl << "Succesfully created file" << endl;
        outFile << "\nSuccessfully created file" << endl;
        //write changes to disk
        dirPtr->flush();
    } else {
        cout << endl << "File already exists" << endl;
        outFile << "\nFile already exists" << endl;
    }

    //writeData = 100 byes
    string writeData = "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789";
    outFile << "Data being written to file(x100): " << writeData << endl;
    int writeResult = -1;
    char *temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    int totalBytes = 0;
    for (int i = 0; i < 100; i++) { //write 10,000 bytes to disk
        writeResult = f->write(temp,
                writeData.size());

        if (writeResult != -1) {
            totalBytes += writeResult;
            cout << endl << totalBytes <<
                    " total bytes of data written successfully" << endl;
            outFile << "\n" << totalBytes << " total bytes of data"
                    "successfully written" << endl;
        } else {
            cout << endl << "Data written unsuccessfully" << endl;
            outFile << "\nData written unsuccessfully" << endl;
        }
        
    }

    //close file
    f->close();

    delete dirPtr;
    delete f;
}

void lab3test5a() {
    //open source
    ofstream outFile;
    outFile.open("test5a.txt");
    bool creat = false;
    bool readAccess = true;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, creat);
    list<Entry> entries;

    //after restarting program...
    cout << "Opening SOURCE" << endl;
    outFile << "Opening SOURCE" << endl;

    File *f;
    try { //open file for reading access
        f = new File("SOURCE", creat, readAccess, &testDisk, dirPtr);
    } catch (FileNotFoundException e) {
        cout << endl << "File not opened" << endl;
        outFile << "\nFile not opened" << endl;
    }

    //create a new file, open it for writing
    cout << "Creating file DEST" << endl;
    outFile << "Creating file DEST" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f2 = new File("DEST", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("DEST") != -1) {
        cout << endl << "Succesfully created file" << endl;
        outFile << "\nSuccessfully created file" << endl;
        //write changes to disk
        dirPtr->flush();
    } else {
        cout << endl << "File already exists" << endl;
        outFile << "\nFile already exists" << endl;
    }

    //copy bytes from SOURCE to DEST
    int bytesToRead = 100;
    for (int i = 0; i < 100; i++) {
        cout << endl << "Bytes to read: " << bytesToRead;
        outFile << "\nBytes to read: " << bytesToRead << endl;

        char readBuffer[bytesToRead + 1];
        for (int i = 0; i < bytesToRead + 1; i++)
            readBuffer[i] = 0;


        int readResult = f->read(readBuffer, bytesToRead);

        if (readResult != -1) {
            cout << endl << readResult <<
                    " bytes of data read successfully" << endl;

            outFile << "\n" << readResult << " bytes of data "
                    "read successfully" << endl;

            cout << "Data read: " << readBuffer << endl;
            outFile << "Data read: " << readBuffer << endl;
        } else {
            cout << endl << "Data read unsuccessfully" << endl;
            outFile << "\nData read unsuccessfully" << endl;
        }
    }

    int writeResult = -1;
    char *writeData = new char[100];
    int totalBytes = 0;
    for (int i = 0; i < 100; i++) { //write those 10,000 bytes to DEST
        writeResult = f2->write(writeData, 100);

        if (writeResult != -1) {
            totalBytes += writeResult;
            cout << endl << totalBytes <<
                    " total bytes of data written successfully to file 2" << endl;
            outFile << "\n" << totalBytes << " total bytes of data"
                    "successfully written to file 2" << endl;
        } else {
            cout << endl << "Data written unsuccessfully to file 2" << endl;
            outFile << "\nData written unsuccessfully to file 2" << endl;
        }
    }

    //close both files
    f->close();
    f2->close();

    if (!dirPtr->flush()) {
        cout << "Error flushing directory!"<<endl;
        outFile << "Error flushing directory!"<<endl;
        exit(1);
    }
    //display directory entries
    entries = dirPtr->listEntries();

    cout << "Start of Directory\n";
    outFile << "Start of Directory\n";
    for (list<Entry>::iterator it = entries.begin(); it != entries.end(); it++) {
        cout << "Name: " << (*it).name << "; FCB number: " << (*it).fcb << "\n";
        outFile << "Name: " << (*it).name << "; FCB number: " << (*it).fcb << "\n";
    }
    cout << "End of Directory\n";
    outFile << "End of Directory\n";


    //open DEST for reading, verify contents
    f2->open(true);

    bytesToRead = 100;
    for (int i = 0; i < 100; i++) {
        cout << endl << "Bytes to read: " << bytesToRead;
        outFile << "\nBytes to read: " << bytesToRead << endl;

        char readBuffer[bytesToRead + 1];
        for (int i = 0; i < bytesToRead + 1; i++)
            readBuffer[i] = 0;


        int readResult = f2->read(readBuffer, bytesToRead);

        if (readResult != -1) {
            cout << endl << readResult <<
                    " bytes of data read successfully from file 2" << endl;

            outFile << "\n" << readResult << " bytes of data "
                    "read successfully from file 2" << endl;

            cout << "Data read: " << readBuffer << endl;
            outFile << "Data read: " << readBuffer << endl;
        } else {
            cout << endl << "Data read unsuccessfully from file 2" << endl;
            outFile << "\nData read unsuccessfully from file 2" << endl;
        }
    }
}

void lab3test5b() {
    ofstream outFile;
    outFile.open("test5b.txt");
    bool creat = true;
    bool readAccess = false;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, creat);

    //create a new file, x, open it for writing
    cout << "Creating file X" << endl;
    outFile << "Creating file X" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f = new File("X", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("X") != -1) {
        cout << endl << "Succesfully created file" << endl;
        outFile << "\nSuccessfully created file" << endl;
        //write changes to disk
        dirPtr->flush();
    } else {
        cout << endl << "File already exists" << endl;
        outFile << "\nFile already exists" << endl;
    }
    //create a new file, y, open it for writing
    cout << "Creating file Y" << endl;
    outFile << "Creating file Y" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f2 = new File("Y", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("Y") != -1) {
        cout << endl << "Succesfully created file" << endl;
        outFile << "\nSuccessfully created file" << endl;
        //write changes to disk
        dirPtr->flush();
    } else {
        cout << endl << "File already exists" << endl;
        outFile << "\nFile already exists" << endl;
    }

    //write bytes into X
    //writeData = 600 byes
    string writeData = "0123456789";
    outFile << "Data being written to file: " << writeData << endl;
    int writeResult = -1;
    char *temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    writeResult = f->write(temp, writeData.size());

    if (writeResult != -1) {
        cout << endl << writeResult <<
                " bytes of data written successfully to X" << endl;
        outFile << "\n" << writeResult << " bytes of data"
                "successfully written to X" << endl;
    } else {
        cout << endl << "Data written unsuccessfully to X" << endl;
        outFile << "\nData written unsuccessfully to X" << endl;
    }

    //write data to Y
    //writeData = 600 byes
    writeData = "0123456789";
    outFile << "Data being written to file: " << writeData << endl;
    writeResult = -1;
    temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    writeResult = f2->write(temp, writeData.size());

    if (writeResult != -1) {
        cout << endl << writeResult <<
                " bytes of data written successfully to Y" << endl;
        outFile << "\n" << writeResult << " bytes of data"
                "successfully written to Y" << endl;
    } else {
        cout << endl << "Data written unsuccessfully to Y" << endl;
        outFile << "\nData written unsuccessfully to Y" << endl;
    }

    //write bytes into X
    //writeData = 600 byes
    writeData = "0123456789";
    outFile << "Data being written to file: " << writeData << endl;
    writeResult = -1;
    temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    writeResult = f->write(temp,
            writeData.size());

    if (writeResult != -1) {
        cout << endl << writeResult <<
                " bytes of data written successfully to X" << endl;
        outFile << "\n" << writeResult << " bytes of data"
                "successfully written to X" << endl;
    } else {
        cout << endl << "Data written unsuccessfully to X" << endl;
        outFile << "\nData written unsuccessfully to X" << endl;
    }

    //write data to Y
    //writeData = 600 byes
    writeData = "0123456789";
    outFile << "Data being written to file(x6): " << writeData << endl;
    writeResult = -1;
    temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    writeResult = f2->write(temp, writeData.size());

    if (writeResult != -1) {
        cout << endl << writeResult <<
                " bytes of data written successfully to Y" << endl;
        outFile << "\n" << writeResult << " bytes of data"
                "successfully written to Y" << endl;
    } else {
        cout << endl << "Data written unsuccessfully to Y" << endl;
        outFile << "\nData written unsuccessfully to Y" << endl;
    }

    //close both files
    f->close();
    f2->close();

    //open both files for reading
    f->open(true);
    f2->open(true);

    //read from X
    int bytesToRead = 10;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    char readBuffer[bytesToRead + 1];
    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;

    int readResult = f->read(readBuffer, bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully from X" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully from X" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully from X" << endl;
        outFile << "\nData read unsuccessfully from X" << endl;
    }

    //read from Y
    bytesToRead = 10;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    readBuffer[bytesToRead + 1];
    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;


    readResult = f2->read(readBuffer, bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully from Y" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully from Y" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully from Y" << endl;
        outFile << "\nData read unsuccessfully from Y" << endl;
    }

    //read from X
    bytesToRead = 10;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    readBuffer[bytesToRead + 1];
    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;

    readResult = f->read(readBuffer, bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully from X" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully from X" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully from X" << endl;
        outFile << "\nData read unsuccessfully from X" << endl;
    }

    //read from Y
    bytesToRead = 10;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    readBuffer[bytesToRead + 1];
    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;


    readResult = f2->read(readBuffer, bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully from Y" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully from Y" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully from Y" << endl;
        outFile << "\nData read unsuccessfully from Y" << endl;
    }

    f->close();
    f2->close();
    f->open(true);
    f2->open(true);

    //read too much from X
    bytesToRead = 21;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    readBuffer[bytesToRead + 1];
    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;

    readResult = f->read(readBuffer, bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully from X" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully from X" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully from X" << endl;
        outFile << "\nData read unsuccessfully from X" << endl;
    }

    //read too much from Y
    bytesToRead = 21;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    readBuffer[bytesToRead + 1];
    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;


    readResult = f2->read(readBuffer, bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully from Y" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully from Y" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully from Y" << endl;
        outFile << "\nData read unsuccessfully from Y" << endl;
    }

    delete f;
    delete f2;
    delete dirPtr;
}

void lab3test6() {
    ofstream outFile;
    outFile.open("test6.txt");

    bool creat;
    bool readAccess;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, true);

    //create a new file, open it for reading
    cout << "Creating file test6" << endl;
    outFile << "Creating file test6" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f = new File("test6", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("test6") != -1) {
        cout << endl << "Succesfully created file" << endl;
        outFile << "\nSuccessfully created file" << endl;
        //write changes to disk ??
        dirPtr->flush();
    } else {
        cout << endl << "File already exists" << endl;
        outFile << "\nFile already exists" << endl;
    }

    string writeData = "012345";
    cout << "Data being written to file: " << writeData << endl;
    outFile << "Data being written to file: " << writeData << endl;
    int writeResult = -1;
    char *temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    writeResult = f->write(temp,
            writeData.size());

    if (writeResult != -1) {
        cout << endl << writeResult <<
                " bytes of data written successfully" << endl;
        outFile << "\n" << writeResult << " bytes of data"
                "successfully written" << endl;
    } else {
        cout << endl << "Data written unsuccessfully" << endl;
        outFile << "\nData written unsuccessfully" << endl;
    }

    //close file
    f->close();

    f->open(false);
    writeData = "6789";
    cout << "Data being written to file: " << writeData << endl;
    outFile << "Data being written to file: " << writeData << endl;
    writeResult = -1;
    temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    writeResult = f->write(temp,
            writeData.size());

    if (writeResult != -1) {
        cout << endl << writeResult <<
                " bytes of data written successfully" << endl;
        outFile << "\n" << writeResult << " bytes of data"
                "successfully written" << endl;
    } else {
        cout << endl << "Data written unsuccessfully" << endl;
        outFile << "\nData written unsuccessfully" << endl;
    }

    //close file
    f->close();


    //open file for reading, read data
    f->open(true);

    int bytesToRead = 10;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    char readBuffer[bytesToRead + 1];
    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;


    int readResult = f->read(readBuffer, bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully" << endl;
        outFile << "\nData read unsuccessfully" << endl;
    }

    f->close();
    f->open(true);

    //try to read past end of file
    //file only has 10 bytes
    bytesToRead = 20;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;


    readResult = f->read(readBuffer, bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully" << endl;
        outFile << "\nData read unsuccessfully" << endl;
    }

    delete f;
}

void lab3test7Before() {
    ofstream outFile;
    outFile.open("test7.txt");

    bool creat;
    bool readAccess;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, true);

    //create a new file, open it for reading
    cout << "Creating file test7" << endl;
    outFile << "Creating file test7" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f = new File("test7", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("test7") != -1) {
        cout << endl << "Succesfully created file" << endl;
        outFile << "\nSuccessfully created file" << endl;
        //write changes to disk ??
        dirPtr->flush();
    } else {
        cout << endl << "File already exists" << endl;
        outFile << "\nFile already exists" << endl;
    }

    string writeData = "012345";
    outFile << "Data being written to file: " << writeData << endl;
    int writeResult = -1;
    char *temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    writeResult = f->write(temp,
            writeData.size());

    if (writeResult != -1) {
        cout << endl << writeResult <<
                " bytes of data written successfully" << endl;
        outFile << "\n" << writeResult << " bytes of data"
                "successfully written" << endl;
    } else {
        cout << endl << "Data written unsuccessfully" << endl;
        outFile << "\nData written unsuccessfully" << endl;
    }

    //close file
    f->close();
}

void lab3test7After() {
    ofstream outFile;
    outFile.open("test7.txt");

    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, false);

    File *f = new File("test7", false, false, &testDisk, dirPtr);

    string writeData = "6789";
    cout << "Data being written to file: " << writeData << endl;
    outFile << "Data being written to file: " << writeData << endl;
    int writeResult = -1;
    char *temp = new char[writeData.size()];
    for (int i = 0; i < writeData.size(); i++)
        temp[i] = writeData.c_str()[i];
    writeResult = f->write(temp, writeData.size());

    if (writeResult != -1) {
        cout << endl << writeResult <<
                " bytes of data written successfully" << endl;
        outFile << "\n" << writeResult << " bytes of data"
                "successfully written" << endl;
    } else {
        cout << endl << "Data written unsuccessfully" << endl;
        outFile << "\nData written unsuccessfully" << endl;
    }

    //close file
    f->close();


    //open file for reading, read data
    f->open(true);

    int bytesToRead = 10;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    char readBuffer[bytesToRead + 1];
    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;

    int readResult = f->read(readBuffer, bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully" << endl;
        outFile << "\nData read unsuccessfully" << endl;
    }

    f->close();
    f->open(true);
    //try to read past end of file
    //file only has 10 bytes
    bytesToRead = 20;
    cout << endl << "Bytes to read: " << bytesToRead;
    outFile << "\nBytes to read: " << bytesToRead << endl;

    for (int i = 0; i < bytesToRead + 1; i++)
        readBuffer[i] = 0;


    readResult = f->read(readBuffer, bytesToRead);

    if (readResult != -1) {
        cout << endl << readResult <<
                " bytes of data read successfully" << endl;

        outFile << "\n" << readResult << " bytes of data "
                "read successfully" << endl;

        cout << "Data read: " << readBuffer << endl;
        outFile << "Data read: " << readBuffer << endl;
    } else {
        cout << endl << "Data read unsuccessfully" << endl;
        outFile << "\nData read unsuccessfully" << endl;
    }

    delete f;
}

void lab3test8() {
    ofstream outFile;
    outFile.open("test8.txt");

    bool creat;
    bool readAccess;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, true);

    //create a new file, open it for reading
    cout << "Creating file test8" << endl;
    outFile << "Creating file test8" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f = new File("test8", creat, readAccess, &testDisk, dirPtr);
    //    files.push_back(*f);

    //maybe have addfile(File) ???

    //make sure file is in directory
    if (dirPtr->findFile("test8") != -1) {
        cout << endl << "Succesfully created file" << endl;
        outFile << "\nSuccessfully created file" << endl;
        //write changes to disk ??
        dirPtr->flush();
    } else {
        cout << endl << "File already exists" << endl;
        outFile << "\nFile already exists" << endl;
    }

    f->close();

    cout << endl << "Entries: ";
    outFile << "\nEntries: ";

    list<Entry> entryList = dirPtr->listEntries();
    while (!entryList.empty()) {
        Entry tempEntry = entryList.front();
        cout << tempEntry.name << ", ";
        outFile << tempEntry.name << ", ";
        entryList.pop_front();
    }

    cout << endl;
    outFile << "\n";

    if (dirPtr->removeFile("test8")) {
        cout << endl << "Successfully deleted file" << endl;
        outFile << "\nSuccessfully deleted file" << endl;
        //dirPtr->flush();
    } else {
        cout << endl << "Couldn't delete file" << endl;
        outFile << "\nCouldn't delete file" << endl;
    }

    cout << endl << "Entries: ";
    outFile << "\nEntries: ";

    list<Entry> entryList2 = dirPtr->listEntries();
    while (!entryList2.empty()) {
        Entry tempEntry = entryList2.front();
        cout << tempEntry.name << ", ";
        outFile << tempEntry.name << ", ";
        entryList2.pop_front();
    }

    cout << endl;
    outFile << "\n";
}

void lab3test9() {
    ofstream outFile;
    outFile.open("test9.txt");

    bool creat;
    bool readAccess;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, true);

    //create a new file, open it for reading
    cout << "Creating file test9" << endl;
    outFile << "Creating file test9" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *fa = new File("test9a", creat, readAccess, &testDisk, dirPtr);
    File *fb = new File("test9b", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("test9a") != -1) {
        cout << endl << "Succesfully created file a" << endl;
        outFile << "\nSuccessfully created file a" << endl;
        dirPtr->flush();
    } else {
        cout << endl << "File already exists a" << endl;
        outFile << "\nFile already exists a" << endl;
    }

    //make sure file is in directory
    if (dirPtr->findFile("test9b") != -1) {
        cout << endl << "Succesfully created file b" << endl;
        outFile << "\nSuccessfully created file b" << endl;
        dirPtr->flush();
    } else {
        cout << endl << "File already exists b" << endl;
        outFile << "\nFile already exists b" << endl;
    }

    cout << endl << "Closing file a." <<endl;
    outFile << endl << "Closing file a." <<endl;
    fa->close();
    cout << endl << "Closing file b." <<endl;
    outFile << endl << "Closing file b." <<endl;
    fb->close();

    cout << endl << "Entries: ";
    outFile << "\nEntries: ";

    list<Entry> entryList = dirPtr->listEntries();
    while (!entryList.empty()) {
        Entry tempEntry = entryList.front();
        cout << tempEntry.name << ", ";
        outFile << tempEntry.name << ", ";
        entryList.pop_front();
    }

    cout << endl;
    outFile << "\n";

    if (dirPtr->removeFile("test9a")) {
        cout << endl << "Successfully deleted file a" << endl;
        outFile << "\nSuccessfully deleted file a" << endl;
        //dirPtr->flush();
    } else {
        cout << endl << "Couldn't delete file a" << endl;
        outFile << "\nCouldn't delete file a" << endl;
    }

    if (dirPtr->removeFile("test9b")) {
        cout << endl << "Successfully deleted file b" << endl;
        outFile << "\nSuccessfully deleted file b" << endl;
        //dirPtr->flush();
    } else {
        cout << endl << "Couldn't delete file b" << endl;
        outFile << "\nCouldn't delete file b" << endl;
    }

    cout << endl << "Entries: ";
    outFile << "\nEntries: ";

    list<Entry> entryList2 = dirPtr->listEntries();
    while (!entryList2.empty()) {
        Entry tempEntry = entryList2.front();
        cout << tempEntry.name << ", ";
        outFile << tempEntry.name << ", ";
        entryList2.pop_front();
    }

    cout << endl;
    outFile << "\n";
}

void lab3test10a() {
    ofstream outFile;
    outFile.open("test10a.txt");

    bool creat;
    bool readAccess;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, true);

    //create a new file, open it for reading
    cout << "Creating file test10" << endl;
    outFile << "Creating file test10" << endl;

    //file is created and added to directory in file ctor
    creat = true;
    readAccess = false;
    File *f = new File("test10", creat, readAccess, &testDisk, dirPtr);

    //make sure file is in directory
    if (dirPtr->findFile("test10") != -1) {
        cout << endl << "Succesfully created file a" << endl;
        outFile << "\nSuccessfully created file a" << endl;
        dirPtr->flush();
    } else {
        cout << endl << "File already exists a" << endl;
        outFile << "\nFile already exists a" << endl;
    }

    cout << endl << "Closing file." <<endl;
    outFile << endl << "Closing file." <<endl;
    f->close();

    cout << endl << "Entries: ";
    outFile << "\nEntries: ";

    list<Entry> entryList = dirPtr->listEntries();
    while (!entryList.empty()) {
        Entry tempEntry = entryList.front();
        cout << tempEntry.name << ", ";
        outFile << tempEntry.name << ", ";
        entryList.pop_front();
    }
    cout << "\nShutting down program." <<endl;
    outFile << "\nShutting down program." <<endl;
    cout << endl;
    outFile << "\n";
    exit(1);
}

void lab3test10b() {
    ofstream outFile;
    outFile.open("test10b.txt");

    bool creat = false;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, creat);

    cout << "Restarting program..."<<endl;
    outFile << "Restarting program..."<<endl;

    cout << endl << "Entries: ";
    outFile << "\nEntries: ";

    list<Entry> entryList = dirPtr->listEntries();
    while (!entryList.empty()) {
        Entry tempEntry = entryList.front();
        cout << tempEntry.name << ", ";
        outFile << tempEntry.name << ", ";
        entryList.pop_front();
    }

    if (dirPtr->removeFile("test10")) {
        cout << endl << "Successfully deleted file." << endl;
        outFile << "\nSuccessfully deleted file." << endl;
        if (!dirPtr->flush()) {
            cout << "Error writing directory to disk." <<endl;
            outFile << "Error writing directory to disk." <<endl;
        }
    } else {
        cout << endl << "Couldn't delete file." << endl;
        outFile << "\nCouldn't delete file." << endl;
    }
    
    cout << endl << "Entries: ";
    outFile << "\nEntries: ";

    entryList = dirPtr->listEntries();
    while (!entryList.empty()) {
        Entry tempEntry = entryList.front();
        cout << tempEntry.name << ", ";
        outFile << tempEntry.name << ", ";
        entryList.pop_front();
    }

    cout << "\nShutting down program." <<endl;
    outFile << "\nShutting down program." <<endl;

    cout << endl;
    outFile << "\n";

    exit(1);
}

void lab3test10c() {
    ofstream outFile;
    outFile.open("test10c.txt");

    bool creat = false;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory* dirPtr = new Directory(&testDisk, creat);

    cout << "Restarting program..."<<endl;
    outFile << "Restarting program..."<<endl;

    cout << endl << "Entries: ";
    outFile << endl << "\nEntries: ";

    list<Entry> entryList = dirPtr->listEntries();
    while (!entryList.empty()) {
        Entry tempEntry = entryList.front();
        cout << tempEntry.name << ", ";
        outFile << tempEntry.name << ", ";
        entryList.pop_front();
    }

    cout << "\n"<<endl;
    outFile << "\n"<<endl;
}

void lab3test11() {
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    Directory dir(&testDisk, false);
    fstream fout("/home/kaleb/Desktop/test11.txt", fstream::out);
    char testMsg[22] = "Feeling stressed yet?";
    int testSize = sizeof (testMsg);
    fout << "Beginning of test.\n\n";

    for (int i = 0; i < 1000; i++) {
        fout << "Iteration number " << i << endl;
        fout << "Creating new file... ";
        File* f = new File("file1", true, false, &testDisk, &dir);
        fout << "file created successfully, open for writing.\n";

        fout << "Writing to file... ";
        if (0 > f->write(testMsg, testSize))
            fout << "Error: could not write to file.\n";
        else
            fout << "write succeeded.\n";

        fout << "Closing file... ";
        if (!f->close())
            fout << "Error: could not close file.\n";
        else
            fout << "file closed successfully.\n";

        fout << "Opening file for writing... ";
        delete f;
        f = new File("file1", false, true, &testDisk, &dir);
        fout << "file open for reading\n";

        fout << "Reading " << testSize << " bytes from file... ";
        char readBuf[testSize];
        int bytesRead = f->read(readBuf, testSize);
        if (0 > bytesRead)
            fout << "Error: could not read from file.\n";
        else {
            fout << "read " << bytesRead << " bytes from file. ";

            bool match = true;
            for (int j = 0; j < testSize; j++)
                if (testMsg[j] != readBuf[j]) {
                    match = false;
                    break;
                }

            if (match)
                fout << "Data in file is correct.\n";
            else
                fout << "Error: file data is incorrect.\n";
        }

        fout << "Attempting to read furthur... ";
        bytesRead = f->read(readBuf, testSize);
        if (0 > bytesRead)
            fout << "Error: could not read from file.\n";
        else
            fout << "read " << bytesRead << " bytes from file.\n";

        fout << "Deleting file... ";
        if (!f->deleteFile())
            fout << "Error: could not delete file.\n";
        else
            fout << "file deleted successfully.\n";

        fout << endl;
        //        fout.flush();
        cout << "Done with loop " << i + 1 << ".\n";
    }

    fout.close();
}

