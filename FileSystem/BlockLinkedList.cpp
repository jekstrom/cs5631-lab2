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
    startBlockNum = END_OF_LIST;
    endBlockNum = END_OF_LIST;
    currentBlockNum = END_OF_LIST;
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
    // Check if end of list has been reached.
    if(END_OF_LIST == currentBlockNum)
        return NULL;

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

bool BlockLinkedList::getNextBlock()
{    
    if(END_OF_LIST != currentBlockNum)
    {
        // Follow pointer to next block number
        Block* curBlkPtr = getCurrentBlock();
        if(curBlkPtr != NULL)
        {
            currentBlockNum = curBlkPtr->getNext();
            delete curBlkPtr;
            return true;
        }
    } else return false;
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
    {
        delete curBlkPtr;
        return true;
    }
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
    
    cout << "Start of list\n";
    while(curBlkPtr != NULL)
    {
        cout << curBlkPtr->getBlockNumber() << ", ";
        this->getNextBlock();
        delete curBlkPtr;
        curBlkPtr = this->getCurrentBlock();
    }
    cout << "\nEnd of list\n";
}

void BlockLinkedList::rewind()
{
    currentBlockNum = startBlockNum;
}

void BlockLinkedList::test()
{
    test1();
    test2();
    test3();
    test4();
    test5();
}

void BlockLinkedList::test1()
{
    cout << "Testing initialize():\n";
    cout << "List length = " << this->getNumberOfBlocks() << "\n";

    cout << "Calling initailize(1)...\n";
    bool initReturn = this->initialize(1);

    cout << "Return of initialize() = " << initReturn << "\n";
    cout << "List length = " << this->getNumberOfBlocks() << "\n";
    cout << "Start Block = " << this->getStartBlockNumber() << "\n";
    cout << "End Block = " << this->getEndBlockNumber() << "\n";
    this->output();
}

void BlockLinkedList::test2()
{
    cout << "\nTesting addBlock():\n";
    this->rewind();
    cout << "List length = " << this->getNumberOfBlocks() << "\n";
    cout << "Start Block = " << this->getStartBlockNumber() << "\n";
    cout << "End Block = " << this->getEndBlockNumber() << "\n";
    this->output();

    Block newBlock(42, Disk::DEFAULT_BLOCK_SIZE);
    cout << "Block newBlock(42, Disk::DEFAULT_BLOCK_SIZE)\n";
    cout << "Calling add(newBlock)...\n";
    bool addReturn = this->addBlock(&newBlock);

    cout << "Return of addBlock() = " << addReturn << "\n";
    cout << "List length = " << this->getNumberOfBlocks() << "\n";
    cout << "Start Block = " << this->getStartBlockNumber() << "\n";
    cout << "End Block = " << this->getEndBlockNumber() << "\n";
    this->output();
}

void BlockLinkedList::test3()
{
    cout << "\nTesting getCurrentBlock(), getNextBlock(), rewind():\n";
    this->rewind();
    Block* curBlkPtr = this->getCurrentBlock();
    cout << "Current Block = " << curBlkPtr->getBlockNumber() << "\n";

    cout << "Calling getNextBlock()...\n";
    this->getNextBlock();
    delete curBlkPtr;

    curBlkPtr = this->getCurrentBlock();
    cout << "Current Block = " << curBlkPtr->getBlockNumber() << "\n";

    cout << "Calling getNextBlock()...\n";
    this->getNextBlock();
    delete curBlkPtr;

    curBlkPtr = this->getCurrentBlock();
    if(this->getCurrentBlock() == NULL)
        cout << "Current Block = NULL\n";
    else
        cout << "Current Block != NULL\n";

    cout << "Calling rewind()...\n";
    this->rewind();
    delete curBlkPtr;

    curBlkPtr = this->getCurrentBlock();
    cout << "Current Block = " << curBlkPtr->getBlockNumber() << "\n";
    delete curBlkPtr;
}

void BlockLinkedList::test4()
{
    cout << "\nTesting unlinkBlock():\n";
    this->rewind();
    Block* curBlkPtr = this->getCurrentBlock();
    cout << "List length = " << this->getNumberOfBlocks() << "\n";
    cout << "Start Block = " << this->getStartBlockNumber() << "\n";
    cout << "End Block = " << this->getEndBlockNumber() << "\n";
    this->output();
    this->rewind();

    cout << "Calling unlinkBlock()...\n";
    Block* unBlkPtr = this->unlinkBlock();

    curBlkPtr = this->getCurrentBlock();
    cout << "List length = " << this->getNumberOfBlocks() << "\n";
    cout << "Start Block = " << this->getStartBlockNumber() << "\n";
    cout << "End Block = " << this->getEndBlockNumber() << "\n";
    cout << "Current Block = " << curBlkPtr->getBlockNumber() << "\n";
    cout << "Unlinked Block = " << unBlkPtr->getBlockNumber() << "\n";
    this->output();

    delete curBlkPtr;
    delete unBlkPtr;
}

void BlockLinkedList::test5()
{
    cout << "\nTesting replace():\n";
    this->rewind();

    Block* curBlkPtr = this->getCurrentBlock();
    cout << "Current Block:\n";
    cout << "Block Number = " << curBlkPtr->getBlockNumber() << ",\n";
    cout << "Block Pointer = " << curBlkPtr->getNext() << ",\n";
    cout << "Block Data = " << curBlkPtr->getBuffer() << "\n";
    delete curBlkPtr;

    cout << "Creating new block with data '12345'...\n";
    Block testBlock(Disk::DEFAULT_BLOCK_SIZE);
    testBlock.setNext(-1);
    for(int i = 1; i <= 5; i++)
        testBlock.setPointer(i, i);
    cout << "New Block's Pointer = " << testBlock.getNext() << "\n";
    cout << "New Block's Data = ";
    for(int i = 1; i <= 5; i++)
        cout << testBlock.getPointer(i);
    cout << "\n";

    cout << "Calling replace()...\n";
    this->replace(&testBlock);

    curBlkPtr = this->getCurrentBlock();
    cout << "Current Block:\n";
    cout << "Block Number = " << curBlkPtr->getBlockNumber() << ",\n";
    cout << "Block Pointer = " << curBlkPtr->getNext() << ",\n";
    cout << "Block Data = ";
    for(int i = 1; i <= 5; i++)
        cout << curBlkPtr->getPointer(i);
    cout << "\n";

}