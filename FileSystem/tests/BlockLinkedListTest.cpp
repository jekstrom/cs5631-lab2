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

void test1(BlockLinkedList* listOne) {
    cout << "Testing initialize():\n";
    cout << "List length = " << listOne->getNumberOfBlocks() << "\n";

    cout << "Calling initailize(1)...\n";
    bool initReturn = listOne->initialize(1);

    cout << "Return of initialize() = " << initReturn << "\n";
    cout << "List length = " << listOne->getNumberOfBlocks() << "\n";
    cout << "Start Block = " << listOne->getStartBlockNumber() << "\n";
    cout << "End Block = " << listOne->getEndBlockNumber() << "\n";
    Block* curBlkPtr = listOne->getCurrentBlock();
    cout << "Current Block = " << curBlkPtr->getBlockNumber() << "\n";
    delete curBlkPtr;
}

void test2(BlockLinkedList* listOne) {
    cout << "Testing addBlock():\n";
    cout << "List length = " << listOne->getNumberOfBlocks() << "\n";
    cout << "Start Block = " << listOne->getStartBlockNumber() << "\n";
    cout << "End Block = " << listOne->getEndBlockNumber() << "\n";
    Block* curBlkPtr1 = listOne->getCurrentBlock();
    cout << "Current Block = " << curBlkPtr1->getBlockNumber() << "\n";

    Block newBlock(42, Disk::DEFAULT_BLOCK_SIZE);
    cout << "Block newBlock(42, Disk::DEFAULT_BLOCK_SIZE)\n";
    cout << "Calling add(newBlock)...\n";
    bool addReturn = listOne->addBlock(&newBlock);

    cout << "Return of addBlock() = " << addReturn << "\n";
    cout << "List length = " << listOne->getNumberOfBlocks() << "\n";
    cout << "Start Block = " << listOne->getStartBlockNumber() << "\n";
    cout << "End Block = " << listOne->getEndBlockNumber() << "\n";
    Block* curBlkPtr2 = listOne->getCurrentBlock();
    cout << "Current Block = " << curBlkPtr2->getBlockNumber() << "\n";

    delete curBlkPtr1;
    delete curBlkPtr2;

//    cout << "%TEST_FAILED% time=0 testname=test2 (BlockLinkedListTest) message=error message sample" << "\n";
}

void test3(BlockLinkedList* listOne) {
    cout << "Testing getCurrentBlock(), getNextBlock(), rewind():\n";
    Block* curBlkPtr = listOne->getCurrentBlock();
    cout << "Current Block = " << curBlkPtr->getBlockNumber() << "\n";

    cout << "Calling getNextBlock()...\n";
    listOne->getNextBlock();
    delete curBlkPtr;

    curBlkPtr = listOne->getCurrentBlock();
    cout << "Current Block = " << curBlkPtr->getBlockNumber() << "\n";

    cout << "Calling getNextBlock()...\n";
    listOne->getNextBlock();
    delete curBlkPtr;

    curBlkPtr = listOne->getCurrentBlock();
    if(listOne->getCurrentBlock() == NULL)
        cout << "Current Block = NULL\n";
    else
        cout << "Current Block != NULL\n";

    cout << "Calling rewind()...\n";
    listOne->rewind();
    delete curBlkPtr;

    curBlkPtr = listOne->getCurrentBlock();
    cout << "Current Block = " << curBlkPtr->getBlockNumber() << "\n";
    delete curBlkPtr;
}

void test4(BlockLinkedList* listOne) {
    cout << "Testing unlinkBlock():\n";
    Block* curBlkPtr = listOne->getCurrentBlock();
    cout << "List length = " << listOne->getNumberOfBlocks() << "\n";
    cout << "Start Block = " << listOne->getStartBlockNumber() << "\n";
    cout << "End Block = " << listOne->getEndBlockNumber() << "\n";
    cout << "Current Block = " << curBlkPtr->getBlockNumber() << "\n";
    delete curBlkPtr;

    cout << "Calling unlinkBlock()...\n";
    Block* unBlkPtr = listOne->unlinkBlock();

    curBlkPtr = listOne->getCurrentBlock();
    cout << "List length = " << listOne->getNumberOfBlocks() << "\n";
    cout << "Start Block = " << listOne->getStartBlockNumber() << "\n";
    cout << "End Block = " << listOne->getEndBlockNumber() << "\n";
    cout << "Current Block = " << curBlkPtr->getBlockNumber() << "\n";
    cout << "Unlinked Block = " << unBlkPtr->getBlockNumber() << "\n";

    delete curBlkPtr;
    delete unBlkPtr;
}

int main(int argc, char** argv) {
    cout << "%SUITE_STARTING% BlockLinkedListTest\n";
    cout << "%SUITE_STARTED% \n\n";

    Disk testDisk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    testDisk.Format();
    BlockLinkedList listOne(&testDisk, Disk::DEFAULT_BLOCK_SIZE);

    cout << "%TEST_STARTED% test1 (BlockLinkedListTest)\n";
    test1(&listOne);
    cout << "%TEST_FINISHED% time=0 test1 (BlockLinkedListTest)\n\n";

    cout << "%TEST_STARTED% test2 (BlockLinkedListTest)\n";
    test2(&listOne);
    cout << "%TEST_FINISHED% time=0 test2 (BlockLinkedListTest)\n\n";

    cout << "%TEST_STARTED% test3 (BlockLinkedListTest)\n";
    test3(&listOne);
    cout << "%TEST_FINISHED% time=0 test3 (BlockLinkedListTest)\n\n";

    cout << "%TEST_STARTED% test4 (BlockLinkedListTest)\n";
    test4(&listOne);
    cout << "%TEST_FINISHED% time=0 test4 (BlockLinkedListTest)" << "\n\n";

    cout << "%SUITE_FINISHED% time=0" << "\n";

    testDisk.Close();

    return (EXIT_SUCCESS);
}

