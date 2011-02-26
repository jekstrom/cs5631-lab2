#include "FreeList.h"
#include "BlockLinkedList.h"
#include "BlockGroup.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

FreeList::FreeList(char* fileName, bool createFreeList){
    // Block size is the number of "usable" bytes in each block
    blockSize = Disk::DEFAULT_BLOCK_SIZE - 4;

    diskPtr = new Disk(fileName,
            DEFAULT_NUMBER_OF_BLOCKS, Disk::DEFAULT_BLOCK_SIZE);

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
            cout << "Error: could not read free list from master block";
        }
    }
    else
    {        
        // Write zeros to entire disk
        diskPtr->Format();

        // Initialize free list data members
        startBlockNum = 1;
        endBlockNum = DEFAULT_NUMBER_OF_BLOCKS - 1;
        numBlocks = DEFAULT_NUMBER_OF_BLOCKS - 1;
        currentBlockNum = startBlockNum;

        // Create and write new master block
        Block newMaster(diskPtr->blockSize());
        newMaster.setPointer(startBlockNum, 0);
        newMaster.setPointer(endBlockNum, 1);
        newMaster.setPointer(numBlocks, 2);
        if(!newMaster.write(diskPtr))
            cout << "Error: Could not write new master block.\n";

        for(int i = startBlockNum; i < endBlockNum; i++)
        {
            Block newBlock(i, Disk::DEFAULT_BLOCK_SIZE);
            newBlock.setNext(i + 1);
            if(!newBlock.write(diskPtr))
                cout << "Error: Write failure while linking free list.\n";
        }

        // should be at last block
        Block lastBlock(endBlockNum, Disk::DEFAULT_BLOCK_SIZE);
        // Through constructor, m_buffer is all zeros, no need to change pointer
        lastBlock.write(diskPtr);
    }
}

FreeList::FreeList(Disk* disk, bool createFreeList){
    // Block size is the number of "usable" bytes in each block
    blockSize = Disk::DEFAULT_BLOCK_SIZE - 4;

    diskPtr = disk;

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
            cout << "Error: could not read free list from master block";
        }
    }
    else
    {
        // Write zeros to entire disk
        diskPtr->Format();

        // Initialize free list data members
        startBlockNum = 1;
        endBlockNum = DEFAULT_NUMBER_OF_BLOCKS - 1;
        numBlocks = DEFAULT_NUMBER_OF_BLOCKS - 1;
        currentBlockNum = startBlockNum;

        // Create and write new master block
        Block newMaster(diskPtr->blockSize());
        newMaster.setPointer(startBlockNum, 0);
        newMaster.setPointer(endBlockNum, 1);
        newMaster.setPointer(numBlocks, 2);
        if(!newMaster.write(diskPtr))
            cout << "Error: Could not write new master block.\n";

        for(int i = startBlockNum; i < endBlockNum; i++)
        {
            Block newBlock(i, Disk::DEFAULT_BLOCK_SIZE);
            newBlock.setNext(i + 1);
            if(!newBlock.write(diskPtr))
                cout << "Error: Write failure while linking free list.\n";
        }

        // should be at last block
        Block lastBlock(endBlockNum, Disk::DEFAULT_BLOCK_SIZE);
        // Through constructor, m_buffer is all zeros, no need to change pointer
        lastBlock.write(diskPtr);
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

bool FreeList::flush()
{
    Block *mb;

    //get the master block
    mb = new Block(MASTER_BLOCK_NUM, diskPtr);

    //changes the local varialbes of the master block
    mb->setPointer(startBlockNum, 0);
    mb->setPointer(endBlockNum, 1);
    mb->setPointer(numBlocks, 2);

    //attempts to write the master block back out
    if (!mb->write(diskPtr)) {
        delete mb;
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

    // update master block
    Block master(MASTER_BLOCK_NUM, diskPtr);
    master.setPointer(endBlockNum, 0);
    master.setPointer(numBlocks, 2);
    if(!master.write(diskPtr)) {
        cout << "Error: could not update master block.\n";
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

    // update master block
    Block master(MASTER_BLOCK_NUM, diskPtr);
    master.setPointer(startBlockNum, 0);
    master.setPointer(numBlocks, 2);
    if(!master.write(diskPtr))
    {
        cout << "Error: could not update master block.\n";
        delete b;
        return NULL;
    }
    
    delete b;
    return bGroup;
}
