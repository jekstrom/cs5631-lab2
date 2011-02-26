/* 
 * File:   main.cpp
 * Author: james
 *
 * Created on February 4, 2011, 4:24 PM
 */
#include <cstdlib>

#include "FileSystemHeaders.h"
#include <iostream>
#include <sstream>

using namespace std;

int lab1Test();
int lab2Test();

/*
 * Performs testing
 */
int main() {
    lab2Test();
    return EXIT_SUCCESS;
}

int lab1Test()
{
    int command;

    BlockGroup* bgPtr;
    FreeList* flPtr;
    while(true)
    {
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
                    if(flPtr != NULL)
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

int lab2Test()
{
    int command = -1;
    bool showMenu = true;
    Disk testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);

    Directory* dirPtr = NULL;
    bool dirOpen = false;

    while(true)
    {
        cout << "CS5631 Team Lab 2 Tester" << endl;
        if(showMenu)
        {
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
//            showMenu = false;
        }
        else
        {
            cout << "..." << endl;
            cout << "9) Redisplay full menu" << endl;
        }
        cout << "Enter Command: ";
        cin >> command;

        if (cin.peek() == '\n' && cin.good()) {
            switch (command) {
                case 0:
                    return (EXIT_SUCCESS);
                    break;
                case 1:
                    if(!dirOpen)
                    {
                        dirPtr = new Directory(&testDisk, true);
                        dirOpen = true;
                        cout << "Directory created.\n";
                    }
                    else
                        cout << "There is already a directory open.\n";
                    break;
                case 2:
                    if(!dirOpen)
                    {
                        dirPtr = new Directory(&testDisk, false);
                        dirOpen = true;
                        cout << "Directory opened.\n";
                    }
                    else
                        cout << "There is already a directory open.\n";
                    break;
                case 3:
                    if(dirOpen)
                    {
                        if(dirPtr->flush())
                            cout << "Directory flush successful.\n";
                        else
                            cout << "Directory flush failed.\n";
                    }
                    else
                        cout << "There is no directory open.\n";
                    break;
                case 4:
                    if(dirOpen)
                    {
                        string name;
                        int fcb;

                        cout << "Enter a name for the new Entry: ";
                        cin >> name;
                        cout << "Enter an FCB number for the new Entry: ";
                        cin >> fcb;

                        if(dirPtr->addFile(name, fcb))
                            cout << "Entry added successfully.\n";
                        else
                            cout << "Failed in adding Entry.\n";
                    }
                    else
                        cout << "There is no directory open.\n";
                    break;
                case 5:
                    if(dirOpen)
                    {
                        string name;
                        int fcb;

                        cout << "Enter a name to search for: ";
                        cin >> name;

                        fcb = dirPtr->findFile(name);
                        if(-1 == fcb)
                            cout << "Entry not found.\n";
                        else
                            cout << "Entry found, contains FCB number " << fcb << ".\n";
                    }
                    else
                        cout << "There is no directory open.\n";
                    break;
                case 6:
                    if(dirOpen)
                    {
                        string oldName;
                        string newName;

                        cout << "Enter the name you wish to replace: ";
                        cin >> oldName;
                        cout << "Enter a new name for the Entry: ";
                        cin >> newName;

                        if(dirPtr->renameFile(oldName, newName))
                            cout << "Entry found and successfully renamed.\n";
                        else
                            cout << "Entry not found.\n";
                    }
                    else
                        cout << "There is no directory open.\n";
                    break;
                case 7:
                    if(dirOpen)
                    {
                        string name;

                        cout << "Enter the name of the Entry to remove: ";
                        cin >> name;

                        if(dirPtr->removeFile(name))
                            cout << "Entry removed successfully.\n";
                        else
                            cout << "Entry not found or couldn't be removed.\n";
                    }
                    else
                        cout << "There is no directory open.\n";
                    break;
                case 8:
                    if(dirOpen)
                    {
                        list<Entry> entries = dirPtr->listEntries();

                        cout << "Start of Directory\n";
                        for(list<Entry>::iterator it = entries.begin(); it != entries.end(); it++)
                            cout << "Name: " << (*it).name << "; FCB number: " << (*it).fcb << "\n";
                        cout << "End of Directory\n";
                    }
                    else
                        cout << "There is no directory open.\n";
                    break;
                case 9:
                    if(dirOpen)
                    {
                        for(int i = 1; i < 15; i++)
                        {
                            stringstream nameStream("");
                            nameStream << "entry" << i;
                            if(dirPtr->addFile(nameStream.str(), i))
                                cout << "Entry " << i << " added successfully.\n";
                            else
                                cout << "Failed in adding Entry " << i << ".\n";
                        }
                    }
                    else
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