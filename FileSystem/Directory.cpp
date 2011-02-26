#include "Directory.h"
#include "FreeList.h"
#include "BlockGroup.h"
#include <math.h>

using namespace std;

Directory::Directory(Disk* disk, bool createNew) {
    this->disk = disk;

    
    Block masterBlock = Block(0, disk);

    if (createNew) {
        freeList = FreeList(disk, true);
        directory = BlockGroup(&freeList);
        directory.addBlock();

        masterBlock.setPointer(directory.getStartBlockNumber(), 3);
        masterBlock.setPointer(directory.getEndBlockNumber(), 4);
        masterBlock.setPointer(directory.getNumberOfBlocks(), 5);
        masterBlock.setPointer(0, 6); // number of entries

        masterBlock.write(disk);
    } else {
        freeList = FreeList(disk, false);

        directory = BlockGroup(masterBlock.getPointer(3)
                , masterBlock.getPointer(4),
                masterBlock.getPointer(5),
                &freeList);

        int numEntries = masterBlock.getPointer(6);

        if (directory.getNumberOfBlocks() > 0) {
            directory.rewind();
            Block* tempBlock = directory.getCurrentBlock();
            
            //traverse directory, store in memory
            do { //search every block
                unsigned char *blockBuffer = tempBlock->getBuffer();

                for (int j = 4; j < Disk::DEFAULT_BLOCK_SIZE - ENTRY_SIZE; j += ENTRY_SIZE) {
                    char fcbBuffer[sizeof (int) ] = {0};
                    char nameBuffer[MAX_NAME_SIZE] = {""};

                    for (int i = 0; i < ENTRY_SIZE; i++) {
                        if (i < sizeof (int))
                            fcbBuffer[i] = blockBuffer[j + i];
                        else
                            nameBuffer[i - sizeof (int) ] = blockBuffer[j + i];
                    }

                    int* fcbPtr = (int*) fcbBuffer;
//                    if (*fcbPtr == -1)
//                        break;

                    entryList.push_back(Entry(*fcbPtr, string(nameBuffer)));

                    // stop if all entries have been read
                    if(numEntries == entryList.size())
                        break;
                }
                delete tempBlock;
                directory.getNextBlock();
                tempBlock = directory.getCurrentBlock();
                
            } while (entryList.size() < numEntries);
        }

    }

}

bool Directory::flush() {
    //search through list
    //construct blocks
    //write to disk
    
    Block masterBlock = Block(0, disk);
//    BlockGroup directory = BlockGroup(masterBlock.getPointer(3),
//            masterBlock.getPointer(4),
//            masterBlock.getPointer(5),
//            &freeList);

    int directorySize = masterBlock.getPointer(5);
    unsigned char* buffer = new unsigned char[Disk::DEFAULT_BLOCK_SIZE];
    int numBlocksNeeded = ((entryList.size() - 1) / ENTRIES_PER_BLOCK) + 1;

    list<Entry> tempList(entryList);

    if (numBlocksNeeded > directorySize) {
        //add blocks to directory
        for (int i = 0; i < numBlocksNeeded - directorySize; i++)
            directory.addBlock();
        if(!freeList.flush())
            return false;
    }
    else if(numBlocksNeeded < directorySize)
    {
        // remove uneeded blocks
        for(int i = directorySize; i > numBlocksNeeded; i--)
        {
            Block* blkPtr = directory.unlinkBlock();
            freeList.addBlock(blkPtr);
            delete blkPtr;
        }
        if(!freeList.flush())
            return false;
    }

    directory.rewind();
    Block *tempBlock = directory.getCurrentBlock();
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
                for (int k = 0; k <= tempList.front().name.length(); k++)
                    buffer[j * ENTRY_SIZE + 2 * sizeof (int) +k] = nameBuffer[k];

                tempList.pop_front();
            } else
                break;
        }

        if(!tempBlock->write(disk))
            return false; // failed in writing to disk
        directory.getNextBlock();
        delete tempBlock;
        tempBlock = directory.getCurrentBlock();
    }
    delete tempBlock;

    // update master block
    masterBlock.setPointer(directory.getStartBlockNumber(), 3);
    masterBlock.setPointer(directory.getEndBlockNumber(), 4);
    masterBlock.setPointer(directory.getNumberOfBlocks(), 5);
    masterBlock.setPointer(entryList.size(), 6);
    return (masterBlock.write(disk) && freeList.flush());
}

bool Directory::addFile(string filename, int fcbNum) {
    if(filename.length() > MAX_NAME_SIZE - 1)
        filename = filename.substr(0, 31);
    Entry newEntry(fcbNum, filename);
    entryList.push_back(newEntry);
    return true;
}

int Directory::findFile(string filename) {
    if(filename.length() > MAX_NAME_SIZE - 1)
        filename = filename.substr(0, 31);

    for (list<Entry>::iterator i = entryList.begin(); i != entryList.end(); i++) {
        if (!i->name.compare(filename)) {//returns 0 if strings are equal
            return i->fcb;
        }
    }
    
    return -1; //file not found
}

bool Directory::renameFile(string filename, string newName) {
    if(filename.length() > MAX_NAME_SIZE - 1)
        filename = filename.substr(0, 31);

    for (list<Entry>::iterator i = entryList.begin(); i != entryList.end(); i++) {
        if (!i->name.compare(filename)) {//returns 0 if strings are equal
            i->name = newName;
            return true;
        }
    }

}

bool Directory::removeFile(string filename) {
    if(filename.length() > MAX_NAME_SIZE - 1)
        filename = filename.substr(0, 31);

    for (list<Entry>::iterator i = entryList.begin(); i != entryList.end(); i++) {
        if (!i->name.compare(filename)) {//returns 0 if strings are equal
            entryList.erase(i);
            return true;
        }
    }
}

list<Entry> Directory::listEntries() {
    return entryList;
}