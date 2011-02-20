//
#include "Directory.h"
#include "FreeList.h"
#include "BlockGroup.h"
#include <math.h>

using namespace std;

Directory::Directory(Disk* disk, bool createNew) {
    this->disk = disk;

    FreeList *freeList = new FreeList(disk, false);
    Block *masterBlock = new Block(0, disk);

    if (createNew) {
        BlockGroup *directory = new BlockGroup(freeList);
        directory->addBlock();
        //set fcb number for the first entry to -1
        directory->getCurrentBlock()->setPointer(-1, 1);

        masterBlock->setPointer(directory->getStartBlockNumber(), 3);
        masterBlock->setPointer(directory->getEndBlockNumber(), 4);
        masterBlock->setPointer(directory->getNumberOfBlocks(), 5);

        masterBlock->write(disk);
    } else {
        BlockGroup *directory = new BlockGroup(masterBlock->getPointer(3)
                , masterBlock->getPointer(4),
                masterBlock->getPointer(5),
                freeList);


        if (directory->getNumberOfBlocks() > 0) {
            Block *tempBlock = new Block(directory->getStartBlockNumber());
            //traverse directory, store in memory
            do { //search every block
                unsigned char *blockBuffer;
                blockBuffer = tempBlock->getBuffer();
                for (int j = 4; j < Disk::DEFAULT_BLOCK_SIZE; j += ENTRY_SIZE) {
                    char fcbBuffer[sizeof (int) ] = {0};
                    char nameBuffer[MAX_NAME_SIZE] = {""};
                    char buffer[ENTRY_SIZE] = {NULL};

                    buffer [j] = blockBuffer[j];

                    for (int i = 0; i < ENTRY_SIZE; i++) {
                        if (i < 4)
                            fcbBuffer[i] = buffer[i];
                        else
                            nameBuffer[i - sizeof (int) ] = buffer[i];
                    }

                    if ((int) fcbBuffer == -1) break;

                    Entry temp((int) fcbBuffer, nameBuffer);

                    entryList.push_back(temp);

                    delete[] fcbBuffer;
                    delete[] nameBuffer;
                    delete[] buffer;
                }
                tempBlock = directory->getCurrentBlock();
            } while (directory->getNextBlock());
        }

    }

}

bool Directory::flush() {
    //search through list
    //construct blocks
    //write to disk
    FreeList *freeList = new FreeList(disk, false);
    Block *masterBlock = new Block(0, disk);
    BlockGroup *directory = new BlockGroup(masterBlock->getPointer(3),
            masterBlock->getPointer(4),
            masterBlock->getPointer(5),
            freeList);

    int directorySize = masterBlock->getPointer(5);
    unsigned char buffer[Disk::DEFAULT_BLOCK_SIZE];
    int numBlocksNeeded = ceil((entryList.size() / 14));

    list<Entry> tempList(entryList);


    if (numBlocksNeeded > directorySize) {
        //add blocks to directory
        for (int i = 0; i < numBlocksNeeded - directorySize; i++)
            directory->addBlock();
    }

    Block *tempBlock = new Block(masterBlock->getPointer(3), disk);
    for (int i = 0; i < numBlocksNeeded; i++) { //for each block
        buffer[0] = tempBlock->getPointer(0);
        for (int j = 0; j < ENTRIES_PER_BLOCK; j++) {
            if (tempList.size() != 0) {
                buffer[j * ENTRY_SIZE + sizeof (int) ] = tempList.front().fcb;
                buffer[j * ENTRY_SIZE + 2 * sizeof (int) ] = *tempList.front().name.c_str();
                tempList.pop_front();
            } else break;
        }

        tempBlock->write(disk);
        directory->getNextBlock();
        tempBlock = directory->getCurrentBlock();
    }
    delete tempList;
    delete tempBlock;
}

bool Directory::addFile(string filename, int fcbNum) {
    Entry newEntry(fcbNum, filename);
    entryList.push_back(newEntry);
}

int Directory::findFile(string filename) {
    list<Entry>::iterator i = entryList.begin();
    for (; i != entryList.end(); i++) {
        if (!i->name.compare(filename)) //returns 0 if strings are equal
            return i->fcb;
    }
    return -1; //file not found
}

bool Directory::renameFile(string filename, string newName) {
    list<Entry>::iterator i = entryList.begin();
    for (; i != entryList.end(); i++) {
        if (!i->name.compare(filename)) {//returns 0 if strings are equal
            i->name = newName;
        }
    }
    return false; //file not found
}

bool Directory::removeFile(string filename) {
    list<Entry>::iterator i = entryList.begin();
    for (; i != entryList.end(); i++) {
        if (!i->name.compare(filename)) //returns 0 if strings are equal
            entryList.erase(i);
    }
    return -1; //file not found
}

list<Entry> Directory::listEntries() {
    return entryList;
}