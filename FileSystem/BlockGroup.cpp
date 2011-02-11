/*
 * File:   BlockGroup.cpp
 * Author: James Ekstrom
 *
 * Created on February 8, 2011, 8:05 PM
 */
#include "BlockGroup.h"
#include "BlockLinkedList.h"

     /**
     * Creates a new BlockGroup.
     * @param bll BlockLinkedList to use to initialize.
     */

BlockGroup::BlockGroup(BlockLinkedList bll) {
    //Not really sure how the FreeList gets added when this constructor is used...
    diskPtr = bll.getDisk();
    numBlocks = 0;
    blockSize = bll.getBlockSize();

    //This is not how to use initialize. Such a call would write over the master block
//    if(bll.initialize(0)) {
//        startBlock = 0;
//        endBlock = 0;
//        numberOfBlocks = 1;
//    }
}

// This constructor makes more sense to me. - Kaleb
BlockGroup::BlockGroup(FreeList* fl) {
    diskPtr = fl->getDisk();
    numBlocks = 0;
    blockSize = fl->getBlockSize();
    motherFreeList = fl;
}

    /**
     * Enables you to re-open a BlockGroup. Assumes that the BlockGroup is not
     * currently open. This enables you to re-open a file on subsequent runs of
     * testing programs, after an initial run when a BlockGroup was created but
     * not deallocated.
     * @param startBlock is the starting block number of the BlockGroup.
     * @param endBlock is the ending block number of the BlockGroup.
     * @param numberOfBlocks is the number of blocks in this BlockGroup.
     * @param motherFreeList is the BlockGroup's originating Free List.
     */
BlockGroup::BlockGroup(int startBlock, int endBlock, int numberOfBlocks,
        FreeList* motherFreeList) {
    this->startBlockNum = startBlock;
    this->endBlockNum = endBlock;
    this->numBlocks = numberOfBlocks;
    this->motherFreeList = motherFreeList;
}

    /**
     * Add another (empty) block to the end of this BlockGroup, from the Free List.
     * You need to make sure the Free List has blocks left in it before calling this method.
     * @return true iff Block could be added.
     */
bool BlockGroup::addBlock() {
    if(this->motherFreeList->getNumberOfBlocks() != 0) {
        Block* curBlk = motherFreeList->unlinkBlock();
        blockLinkedList->addBlock(curBlk);
        delete curBlk;
        return true;
    }
    else {
        printf("BlockGroup::addBlock Failed, FreeList empty.");
        return false;
    }
}