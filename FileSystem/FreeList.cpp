#include "FreeList.h"
#include "BlockLinkedList.h"
#include "BlockGroup.h"
#include <iostream>

using namespace std;

FreeList::FreeList(char* fileName, bool createFreeList){
    blockSize = Disk::DEFAULT_BLOCK_SIZE;

    diskPtr = new Disk(fileName,
            DEFAULT_NUMBER_OF_BLOCKS, blockSize);

    if(!createFreeList)
    {
        try
        {
            // Read free list info from master block
            Block masterBlock(MASTER_BLOCK_NUM, FreeList::diskPtr);
            startBlockNum = masterBlock.getPointer(0);
            endBlockNum = masterBlock.getPointer(1);
            numBlocks = masterBlock.getPointer(2);
            this->rewind();
        }
        catch(CannotReadException e)
        {
            // something, something, error handling
        }
    }
    else
    {
        // Write zeros to entire disk
        diskPtr->Format();

        // Create and write new master block
        Block newMaster(Disk::DEFAULT_BLOCK_SIZE);
        newMaster.setPointer(1,0);
        newMaster.setPointer(DEFAULT_NUMBER_OF_BLOCKS - 1, 1);
        newMaster.setPointer(DEFAULT_NUMBER_OF_BLOCKS - 1, 2);
        if(!newMaster.write(diskPtr))
            cout << "Error: Could not write new master block.\n";
    }
}

bool FreeList::close() {
    Block *mb;

    //get the master block
    mb = new Block(MASTER_BLOCK_NUM, diskPtr);

    //changes the local varialbes of the master block
    mb->setPointer(startBlockNum, 0);
    mb->setPointer(endBlockNum, 1);
    mb->setPointer(numBlocks, 2);

    //attempts to write the master block back out
    if (!mb->write(diskPtr) || !diskPtr->Close()) {
        delete mb;
        cout << "Error: FreeList::close()\n" << endl;
        return false;
    } else {
        delete mb;
        return true;
    }
}

void FreeList::returnBlocks(BlockLinkedList* bll) {
    Block *b;

    while (bll->getNumberOfBlocks() != 0) {
        b = bll->unlinkBlock();
        addBlock(b);
        delete b;
    }
}

BlockGroup* FreeList::createNew() {
    Block *b;
    BlockGroup *bGroup;
    int bNumber = 0;

    b = NULL;
    bGroup = new BlockGroup(this);

    //gets first block off freelist
    b = unlinkBlock();
    if (b == NULL) {
        delete bGroup;
        return NULL;
    }
    startBlockNum = b->getNext();

    //init block group
    bNumber = b->getBlockNumber();
    bGroup->initialize(bNumber);
    
    delete b;
    return bGroup;
}

void FreeList::test() {
    //Todo; waiting on testing of superclass
}
