#include "Directory.h"
#include "FreeList.h"
#include "BlockGroup.h"
#include <math.h>

using namespace std;

Directory::Directory(Disk* disk, bool createNew) {
    this->disk = disk;

    FreeList freeList = FreeList(disk, false);
    Block masterBlock = Block(0, disk);

    if (createNew) {
        BlockGroup directory = BlockGroup(&freeList);
        directory.addBlock();

        masterBlock.setPointer(directory.getStartBlockNumber(), 3);
        masterBlock.setPointer(directory.getEndBlockNumber(), 4);
        masterBlock.setPointer(directory.getNumberOfBlocks(), 5);

        masterBlock.write(disk);
    } else {
        BlockGroup directory = BlockGroup(masterBlock.getPointer(3)
                , masterBlock.getPointer(4),
                masterBlock.getPointer(5),
                &freeList);


        if (directory.getNumberOfBlocks() > 0) {
            Block* tempBlock = new Block(directory.getStartBlockNumber(), disk); // handle exception
            directory.getNextBlock();
            //traverse directory, store in memory
            do { //search every block
                unsigned char *blockBuffer;
                blockBuffer = tempBlock->getBuffer();

                for (int j = 4; j < Disk::DEFAULT_BLOCK_SIZE; j += ENTRY_SIZE) {
                    char fcbBuffer[sizeof (int) ] = {0};
                    char nameBuffer[MAX_NAME_SIZE] = {""};

                    for (int i = 0; i < ENTRY_SIZE; i++) {
                        if (i < sizeof (int))
                            fcbBuffer[i] = blockBuffer[j + i];
                        else
                            nameBuffer[i - sizeof (int) ] = blockBuffer[j + i];
                    }

                    int* fcbPtr = (int*) fcbBuffer;
                    if (*fcbPtr == -1)
                        break;

                    entryList.push_back(Entry(*fcbPtr, string(nameBuffer)));
                }
                delete tempBlock;
                tempBlock = directory.getCurrentBlock();
            } while (directory.getNextBlock());
        }

    }

}

bool Directory::flush() {
    //search through list
    //construct blocks
    //write to disk
    FreeList freeList = FreeList(disk, false);
    Block masterBlock = Block(0, disk);
    BlockGroup directory = BlockGroup(masterBlock.getPointer(3),
            masterBlock.getPointer(4),
            masterBlock.getPointer(5),
            &freeList);

    int directorySize = masterBlock.getPointer(5);
    unsigned char* buffer = new unsigned char[Disk::DEFAULT_BLOCK_SIZE];
    int numBlocksNeeded = ceil((entryList.size() / 14));

    list<Entry> tempList(entryList);

    if (numBlocksNeeded > directorySize) {
        //add blocks to directory
        for (int i = 0; i < numBlocksNeeded - directorySize; i++)
            directory.addBlock();
    }

    Block *tempBlock = new Block(masterBlock.getPointer(3), disk);
    //Block newBlock = Block(tempBlock->getBlockNumber());
    for (int i = 0; i < numBlocksNeeded; i++) { //for each block
        // newBlock.setNext() = tempBlock->getNext(0);
        buffer = tempBlock->getBuffer();
        for (int j = 0; j < ENTRIES_PER_BLOCK; j++) {
            if (tempList.size() != 0) {
                //buffer[j * ENTRY_SIZE + sizeof (int) ] = tempList.front().fcb;
                tempBlock->setPointer(tempList.front().fcb,
                        (j * ENTRY_SIZE + sizeof (int)) / 4);

                const char* nameBuffer = tempList.front().name.c_str();
                for (int k = 0; k < sizeof (nameBuffer); k++)
                    buffer[j * ENTRY_SIZE + 2 * sizeof (int) +k] = nameBuffer[k];

                tempList.pop_front();
            } else
                break;
        }

        tempBlock->write(disk);
        directory.getNextBlock();
        delete tempBlock;
        tempBlock = directory.getCurrentBlock();
        delete buffer;
    }
    delete tempBlock;
}

bool Directory::addFile(string filename, int fcbNum) {
    if (filename.size() < 33) {
        Entry newEntry(fcbNum, filename);
        entryList.push_back(newEntry);
        return true;
    } else //filename too long
        return false;
}

int Directory::findFile(string filename) {
    if (filename.size() < 33) {
        for (list<Entry>::iterator i = entryList.begin(); i != entryList.end(); i++) {
            if (!i->name.compare(filename)) {//returns 0 if strings are equal
                return i->fcb;
            }
        }
    } else
        return -1; //file not found
}

bool Directory::renameFile(string filename, string newName) {
    if (filename.size() < 33) {
        for (list<Entry>::iterator i = entryList.begin(); i != entryList.end(); i++) {
            if (!i->name.compare(filename)) {//returns 0 if strings are equal
                i->name = newName;
                return true;
            }
        }
    } else
        return false; //file not found
}

bool Directory::removeFile(string filename) {
    if (filename.size() < 33) {
        for (list<Entry>::iterator i = entryList.begin(); i != entryList.end(); i++) {
            if (!i->name.compare(filename)) {//returns 0 if strings are equal
                entryList.erase(i);
                return true;
            }
        }
    } else
        return false; //file not found
}

list<Entry> Directory::listEntries() {
    return entryList;
}