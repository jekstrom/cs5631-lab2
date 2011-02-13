/* 
 * File:   main.cpp
 * Author: james
 *
 * Created on February 4, 2011, 4:24 PM
 */

#include <cstdlib>

#include "BlockTester.h"
#include "FileSystemHeaders.h"
#include "BlockGroupTester.h"

using namespace std;

/*
 *
 */
int main() {
    //BlockTester test1;

    Disk testDisk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    testDisk.Format();
    BlockLinkedList listOne(&testDisk, Disk::DEFAULT_BLOCK_SIZE);
    listOne.test();

    //    BlockGroupTester();

    return 0;
}

void tester() {
    int command;

    while (command != 0) {


        cout << "CS5631 Team Lab 1 Tester" << endl;
        cout << "0) Exit Program" << endl;
        cout << "1) create and format FreeList" << endl;
        cout << "2) open and don't format FreeList" << endl;
        cout << "3) close FreeList" << endl;
        cout << "4) output block number in FreeList" << endl;
        cout << "5) create BlockGroup" << endl;
        cout << "6) return BlockGroup to FreeList" << endl;
        cout << "7) add block to BlockGroup" << endl;
        cout << "8) output block numbers of BlockGroup" << endl;
        cout << "9) re-open BlockGroup" << endl;
        cout << "Enter Command: ";
        cin >> command;

        if (cin.peek() == '\n' && cin.good()) {
            switch (command) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
                case 7:
                    break;
                case 8:
                    break;
                case 9:
                    break;
                default:
                    cout << "Bad input" << endl;
                    break;
            }
        } else {
            cout << "Bad input" << endl;
            tester();
        }
    }
}

