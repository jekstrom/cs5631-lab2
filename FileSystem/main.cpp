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

//    Disk testDisk("testDisk", FreeList::DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);
//    testDisk.Format();
//    BlockLinkedList listOne(&testDisk, Disk::DEFAULT_BLOCK_SIZE);
//    listOne.test();

    BlockGroupTester();

    return 0;
}

