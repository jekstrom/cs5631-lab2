/*
 * File:   BlockGroup.cpp
 * Author: James Ekstrom
 *
 * Created on February 8, 2011, 8:05 PM
 */
#include "BlockGroup.h"
#include "BlockLinkedList.h"


BlockGroup::BlockGroup(BlockLinkedList bll) {
    diskPtr = bll.getDisk();
    numBlocks = 0;
    blockSize = bll.getBlockSize();
}

BlockGroup::BlockGroup(FreeList* fl) {
    if(fl != NULL)
    {
        diskPtr = fl->getDisk();
        numBlocks = 0;
        blockSize = fl->getBlockSize();
        motherFreeList = fl;
        startBlockNum = 0;
        endBlockNum = 0;
        currentBlockNum = startBlockNum;
    }
}

BlockGroup::BlockGroup(int startBlock, int endBlock, int numberOfBlocks,
        FreeList* motherFreeList) {
    this->startBlockNum = startBlock;
    this->endBlockNum = endBlock;
    this->numBlocks = numberOfBlocks;
    this->motherFreeList = motherFreeList;
    this->diskPtr = this->motherFreeList->getDisk();
    this->blockSize = diskPtr->blockSize() - 4;
    this->currentBlockNum = startBlockNum;
}

BlockGroup::BlockGroup()
{
}

bool BlockGroup::addBlock() {
    if(this->motherFreeList->getNumberOfBlocks() > 0) {
        // If block group is empty, initialize with first block from free list
        if(numBlocks == 0)
        {
            Block* firstBlk = motherFreeList->unlinkBlock();
            int startNum = firstBlk->getBlockNumber();
            delete firstBlk;
            return initialize(startNum);
        }

        // Otherwise take and add first block from free list
        Block* curBlk = motherFreeList->unlinkBlock();
        bool addStatus = BlockLinkedList::addBlock(curBlk);
        delete curBlk;
        return addStatus;
    }
    else {
        printf("BlockGroup::addBlock Failed, FreeList empty.");
        return false;
    }
}
