/*
 * File:   BlockLinkedList.cpp
 * Author: Kaleb Jungclaus
 *
 * Created on February 6, 2011, 4:10 PM
 */

#include "BlockLinkedList.h"

using namespace std;

BlockLinkedList::BlockLinkedList(Disk* disk, int blockSize)
{
    this->disk = disk;
    this->blockSize = blockSize;
    this->numBlocks = 0;
    this->startBlockNum = 0;
    this->endBlockNum = 0;
}

BlockLinkedList::BlockLinkedList(BlockLinkedList* b)
{
    this->disk = b->getDisk();
    this->blockSize = b->getBlockSize();
    this->numBlocks = 0;
    this->startBlockNum = 0;
    this->endBlockNum = 0;
}

bool BlockLinkedList::addBlock(Block* newBlock)
{
    //TODO: change pointer
    newBlock->clearBuffer();


}

Block* BlockLinkedList::getCurrentBlock()
{
    if(currentBlockNum <= endBlockNum)
        return &currentBlock;
    else
        return NULL;
}

void BlockLinkedList::getNextBlock()
{
    currentBlockNum++;
    if(currentBlockNum <= endBlockNum)
        currentBlock = Block(currentBlockNum, disk);
}

