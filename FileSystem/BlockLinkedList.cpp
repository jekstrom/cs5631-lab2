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

bool BlockLinkedList::addBlock(Block* newBlock) throw(CannotReadException)
{    
    // Update pointer in previous last node
    try
    {
        Block endBlk(endBlockNum, diskPtr);
        endBlockNum = newBlock->getBlockNumber();
        endBlk.setNext(endBlockNum);
    }
    catch(CannotReadException e)
    {
        throw e;
        return false;
    }

    // Clear and write the new block    
    newBlock->clearBuffer();
    newBlock->setNext(END_OF_LIST);
    return newBlock->write(diskPtr);
}

Block* BlockLinkedList::getCurrentBlock()
{
    // Update current block pointer if needed
    if(!currentCalled)
    {
        if(END_OF_LIST == currentBlockNum)
            currentBlockPtr = NULL;
        else
        {
            try
            {
                currentBlockPtr = new Block(currentBlockNum, diskPtr);
            }
            catch(CannotReadException e)
            {
                return NULL;
            }
        }

        currentCalled = true;
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
            currentCalled = false;
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
    currentCalled = false;

    // Create and write the new first block on the disk
        Block firstBlk(blockNumber, Disk::DEFAULT_BLOCK_SIZE);
        firstBlk.clearBuffer();
        firstBlk.setNext(END_OF_LIST);
        return firstBlk.write(diskPtr);
}

bool BlockLinkedList::replace(Block* blk)
{
    //TODO: figure out just wth this function is supposed to do
}

Block* BlockLinkedList::unlinkBlock()
{
    // Change starting block number to the block number of the 2nd block
    Block* blockPtr;
    try
    {
        blockPtr = new Block(startBlockNum, diskPtr);
        if(currentBlockNum == startBlockNum) // change current block if needed
        {
            currentBlockNum = blockPtr->getNext();
            currentCalled = false;
        }
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

    currentCalled = false;
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

    currentCalled = false;
}