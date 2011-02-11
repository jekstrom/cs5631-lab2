/*
 * File:   BlockLinkedList.cpp
 * Author: Kaleb Jungclaus
 *
 * Created on February 6, 2011, 4:10 PM
 */

#include "BlockLinkedList.h"
#include <iostream>

using namespace std;

BlockLinkedList::BlockLinkedList()
{
    // Not actually used, but having it here makes the compiler happy
}

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

bool BlockLinkedList::addBlock(Block* newBlock) throw(CannotReadException)
{    
    // Update pointer in previous last node
    try
    {
        Block endBlk(endBlockNum, diskPtr);
        endBlockNum = newBlock->getBlockNumber();
        endBlk.setNext(endBlockNum);
        if(!endBlk.write(diskPtr))
            return false;
    }
    catch(CannotReadException e)
    {
        throw e;
        return false;
    }

    // Clear and write the new block    
    newBlock->clearBuffer();
    newBlock->setNext(END_OF_LIST);
    if(newBlock->write(diskPtr))
    {
        numBlocks++;
        return true;
    }
    else
        return false;
}

Block* BlockLinkedList::getCurrentBlock()
{
    // Attempt to read block from disk
    Block* currentBlockPtr;
    try
    {
        currentBlockPtr = new Block(currentBlockNum, diskPtr);
    }
    catch(CannotReadException e)
    {
        return NULL;
    }

    return currentBlockPtr;
}

void BlockLinkedList::getNextBlock()
{
    // Follow pointer to next block number
    if(END_OF_LIST != currentBlockNum)
    {
        Block* curBlkPtr = getCurrentBlock();
        if(curBlkPtr != NULL)
        {
            currentBlockNum = curBlkPtr->getNext();
            delete curBlkPtr;
        }
    }
    // If end of list has been reached, do nothing
}

bool BlockLinkedList::initialize(int blockNumber)
{
    // Initialize data members of the list
    numBlocks = 1;
    startBlockNum = blockNumber;
    endBlockNum = blockNumber;
    currentBlockNum = blockNumber;

    // Create and write the new first block on the disk
    Block firstBlk(blockNumber, Disk::DEFAULT_BLOCK_SIZE);
    firstBlk.clearBuffer();
    firstBlk.setNext(END_OF_LIST);
    return firstBlk.write(diskPtr);
}

bool BlockLinkedList::replace(Block* blk)
{
    Block* curBlkPtr = getCurrentBlock();
    int curPointer = curBlkPtr->getNext();

    // Rewrite buffer, restore pointer
    curBlkPtr->setBuffer(blk->getBuffer());
    curBlkPtr->setNext(curPointer);
    if(!curBlkPtr->write(diskPtr))
    {
        cout << "Error: could not replace block\n";
        delete curBlkPtr;
        return false;
    }
    else
        return true;
}

Block* BlockLinkedList::unlinkBlock()
{
    if(0 == numBlocks)
        return NULL; // List is empty, no block to return

    Block* blockPtr;
    try
    {
        blockPtr = new Block(startBlockNum, diskPtr);
        if(currentBlockNum == startBlockNum) // change current block if needed
            currentBlockNum = blockPtr->getNext();

        // Change starting block number to the block number of the new 1st block
        startBlockNum = blockPtr->getNext();
    }
    catch(CannotReadException e)
    {
        return NULL;
    }
    
    //Decrement list length
    numBlocks--;

    return blockPtr;
}

int BlockLinkedList::countBlocks()
{
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

    delete curBlkPtr;
    return count;
}

void BlockLinkedList::output()
{
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

void BlockLinkedList::rewind()
{
    currentBlockNum = startBlockNum;
}