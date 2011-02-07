/* 
 * File:   newsimpletest.cpp
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
    cout << "Testing initialize():" << "\n";
    cout << "List length = " << listOne.getNumberOfBlocks() << "\n";

    cout << "Calling initailize(1)...\n";
    listOne.initialize(1);

    cout << "List length = " << listOne.getNumberOfBlocks() << "\n";
    cout << "Start Block = " << listOne.getStartBlockNumber() << "\n";
    cout << "End Block = " << listOne.getEndBlockNumber() << "\n";
}

void test2() {
    cout << "newsimpletest test 2" << "\n";
    cout << "%TEST_FAILED% time=0 testname=test2 (newsimpletest) message=error message sample" << "\n";
}

int main(int argc, char** argv) {
    cout << "%SUITE_STARTING% newsimpletest" << "\n";
    cout << "%SUITE_STARTED%" << "\n";

    testDisk = Disk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
    listOne = BlockLinkedList(&testDisk, Disk::DEFAULT_BLOCK_SIZE);

    cout << "%TEST_STARTED% test1 (newsimpletest)" << "\n";
    test1();
    cout << "%TEST_FINISHED% time=0 test1 (newsimpletest)" << "\n\n";

    cout << "%TEST_STARTED% test2 (newsimpletest)\n" << "\n";
    test2();
    cout << "%TEST_FINISHED% time=0 test2 (newsimpletest)" << "\n";

    cout << "%SUITE_FINISHED% time=0" << "\n";

    return (EXIT_SUCCESS);
}

