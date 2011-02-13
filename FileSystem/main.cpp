/* 
 * File:   main.cpp
 * Author: james
 *
 * Created on February 4, 2011, 4:24 PM
 */
#include <cstdlib>

#include "FileSystemHeaders.h"
#include <iostream>

using namespace std;

/*
 * Performs testing
 */
int main() {

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
                        cout "There is no free list open.\n";
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

