/*
 * File:   BlockLinkedList.cpp
 * Author: Kaleb Jungclaus
 *
 * Created on February 6, 2011, 4:10 PM
 */

#include "BlockLinkedList.h"
#include <iostream>

using namespace std;

BlockLinkedList::BlockLinkedList(Disk* disk, int blkSize)
{
    diskPtr = disk;
    blockSize = blkSize - 4;
    numBlocks = 0;
}

BlockLinkedList::BlockLinkedList(BlockLinkedList* b)
{
    diskPtr = b->getDisk();
    blockSize = b->getBlockSize();
    numBlocks = 0;
}

bool BlockLinkedList::addBlock(Block* newBlock)
{    
    // Update pointer in previous last node
    Block endBlk(endBlockNum, diskPtr);
    endBlockNum = newBlock->getBlockNumber();
    endBlk.setNext(endBlockNum);

    // Clear and write the new block
    newBlock->setNext(END_OF_LIST);
    newBlock->clearBuffer();
    newBlock->write(diskPtr);
}

Block* BlockLinkedList::getCurrentBlock()
{
    // Return NULL if the current block is beyond the end of the list
    if(index <= numBlocks)
        return new Block(currentBlockNum, diskPtr);
    else
        return NULL;
}

void BlockLinkedList::getNextBlock()
{
    // Increment index, follow pointer in current block
    index++;
    Block curBlk(currentBlockNum, diskPtr);
    currentBlockNum = curBlk.getNext();
}

Disk* BlockLinkedList::getDisk()
{
    return diskPtr;
}

bool BlockLinkedList::initialize(int blockNumber)
{
    // Initialize data members of the list
    numBlocks = 1;
    index = 1;
    startBlockNum = blockNumber;
    endBlockNum = blockNumber;
    currentBlockNum = blockNumber;

    // Clear and write the new first block on the disk
    Block firstBlk(blockNumber, diskPtr);
    firstBlk.setNext(END_OF_LIST);
    firstBlk.clearBuffer();
    firstBlk.write(diskPtr);
}

bool BlockLinkedList::replace(Block* blk)
{
    //TODO: figure out just wth this function is supposed to do
}

Block* BlockLinkedList::unlinkBlock()
{
    // Change starting block number to the block number of the 2nd block
    Block* blockPtr = new Block(startBlockNum, diskPtr);
    startBlockNum = blockPtr->getNext();
    return blockPtr;
}

int BlockLinkedList::countBlocks()
{
    index = 1;
    currentBlockNum = startBlockNum;
    int count = 0;

    Block* curBlkPtr = this->getCurrentBlock();
    while(curBlkPtr != NULL)
    {
        count++;
        this->getNextBlock();
        delete curBlkPtr;
        curBlkPtr = this->getCurrentBlock();
    }

    return count;
}

void BlockLinkedList::output()
{
    index = 1;
    currentBlockNum = startBlockNum;

    Block* curBlkPtr = this->getCurrentBlock();
    while(curBlkPtr != NULL)
    {
        cout << curBlkPtr->getBlockNumber() << "\n";
        this->getNextBlock();
        delete curBlkPtr;
        curBlkPtr = this->getCurrentBlock();
    }
}