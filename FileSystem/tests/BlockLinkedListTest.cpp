/* 
 * File:   BlockLinkedListTest.cpp
 * Author: kaleb
 *
 * Created on Feb 7, 2011, 11:41:39 AM
 */

#include <stdlib.h>
#include <iostream>
#include "FileSystemHeaders.h"

using namespace std;

/*
 * Testing for BlockLinkedList class
 */

Disk testDisk;
BlockLinkedList listOne;

void test1() {
    cout << "Testing initialize():\n";
    cout << "List length = " << listOne.getNumberOfBlocks() << "\n";

    cout << "Calling initailize(1)...\n";
    listOne.initialize(1);

    cout << "List length = " << listOne.getNumberOfBlocks() << "\n";
    cout << "Start Block = " << listOne.getStartBlockNumber() << "\n";
    cout << "End Block = " << listOne.getEndBlockNumber() << "\n";
    Block* curBlkPtr = listOne.getCurrentBlock();
    cout << "Current Block = " << curBlkPtr->getBlockNumber();
    delete curBlkPtr;
}

void test2() {
    cout << "Testing addBlock():\n";
    cout << "List length = " << listOne.getNumberOfBlocks() << "\n";
    cout << "Start Block = " << listOne.getStartBlockNumber() << "\n";
    cout << "End Block = " << listOne.getEndBlockNumber() << "\n";

    Block newBlock(42, Disk::DEFAULT_BLOCK_SIZE);
    cout << "Block newBlock(42, Disk::DEFAULT_BLOCK_SIZE)\n";
    cout << "Calling add(newBlock)...\n";

    cout << "List length = " << listOne.getNumberOfBlocks() << "\n";
    cout << "Start Block = " << listOne.getStartBlockNumber() << "\n";
    cout << "End Block = " << listOne.getEndBlockNumber() << "\n";

//    cout << "%TEST_FAILED% time=0 testname=test2 (BlockLinkedListTest) message=error message sample" << "\n";
}

void test3() {

}

int main(int argc, char** argv) {
    cout << "%SUITE_STARTING% BlockLinkedListTest" << "\n";
    cout << "%SUITE_STARTED%" << "\n";

    testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    listOne = BlockLinkedList(&testDisk, Disk::DEFAULT_BLOCK_SIZE);

    cout << "%TEST_STARTED% test1 (BlockLinkedListTest)" << "\n";
    test1();
    cout << "%TEST_FINISHED% time=0 test1 (BlockLinkedListTest)" << "\n\n";

    cout << "%TEST_STARTED% test2 (BlockLinkedListTest)\n" << "\n";
    test2();
    cout << "%TEST_FINISHED% time=0 test2 (BlockLinkedListTest)" << "\n\n";

    cout << "%SUITE_FINISHED% time=0" << "\n";

    return (EXIT_SUCCESS);
}

