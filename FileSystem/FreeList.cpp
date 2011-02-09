#include "FreeList.h"
#include "BlockLinkedList.h"
#include "BlockGroup.h"

using namespace std;

FreeList::FreeList(char* fileName, bool createFreeList) {
    Block *b;
    blockSize = Disk::DEFAULT_BLOCK_SIZE;

    diskPtr = new Disk(fileName,
            DEFAULT_NUMBER_OF_BLOCKS, blockSize);




    //TODO: finish


    

}

bool FreeList::close() {
    Block *mb;

    //get the master block
    mb = new Block(0, FreeList::diskPtr);

    //changes the local varialbes of the master block
    mb->setPointer(startBlockNum, 0);
    mb->setPointer(endBlockNum, 1);

    //attempts to write the master block back out
    if (!mb->write(diskPtr) || !diskPtr.Close()) {
        delete mb;
        cout << "Error: FreeList::close()" << endl;
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
    bGroup = new BlockGroup();

    //gets first block off freelist
    b = unlinkBlock();
    if (b == NULL) {
        delete bGroup;
        return NULL;
    }

    //init block
    bNumber = b->getBlockNumber();
    b->clearBuffer();
    b->setPointer(0, 0);

    //init block group
    bGroup->startBlockNum = bNumber;
    bGroup->endBlockNum = bNumber;
    bGroup->blockSize = blockSize;
    bGroup->diskPtr = diskPtr;
    bGroup->numBlocks = 1;


    //attempts to write the block to disk
    if (!b->write(diskPtr)) {
        delete b;
        delete bGroup;
        cout << "Error: FreeList::createNew() - write" << endl;
        return NULL;
    } else {
        delete b;
        return bGroup;
    }
}

static void FreeList::test() {
    //Todo; waiting on testing of superclass
}
