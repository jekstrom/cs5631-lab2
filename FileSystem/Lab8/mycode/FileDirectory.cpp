// Kaleb Jungclaus, James Ekstrom, Matt Fischer

#include "FileDirectory.h"
#include "FreeList.h"
#include "BlockGroup.h"
#include "CannotReadException.h"
#include <iostream>

using namespace std;

FileDirectory::FileDirectory(Disk* disk, bool createNew) throw (CannotReadException) {
    pthread_mutex_init(&dirMutex, NULL);
    pthread_mutex_lock (&dirMutex);
    this->disk = disk;    

    if (createNew) {
        try{
            freeList = FreeList(disk, true);
        }
        catch(CannotReadException e){
            throw e;
        }
        Block masterBlock = Block(0, disk);
        directory = BlockGroup(&freeList);
        if(!directory.addBlock())
            cout << "Error: Failed in creating new Directory.\n";

        masterBlock.setPointer(directory.getStartBlockNumber(), 3);
        masterBlock.setPointer(directory.getEndBlockNumber(), 4);
        masterBlock.setPointer(directory.getNumberOfBlocks(), 5);
        masterBlock.setPointer(0, 6); // number of entries

        if(!masterBlock.write(disk) || !freeList.flush())
            cout << "Error: Failed in creating new Directory.\n";
    } else {
        try{
            freeList = FreeList(disk, false);
        }
        catch(CannotReadException e){
            throw e;
        }

        Block masterBlock = Block(0, disk);
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

    pthread_mutex_unlock (&dirMutex);

}

bool FileDirectory::flush() {
    //search through list
    //construct blocks
    //write to disk
    pthread_mutex_lock (&dirMutex);
    Block masterBlock(Disk::DEFAULT_BLOCK_SIZE);
    try{
        masterBlock = Block(0, disk);
        freeList = FreeList(disk, false);
    }
    catch(CannotReadException e){
        return false;
    }

    int directorySize = masterBlock.getPointer(5);
    unsigned char* buffer = new unsigned char[Disk::DEFAULT_BLOCK_SIZE];
    int numBlocksNeeded;
    if (entryList.size() > 0)
        numBlocksNeeded = ((entryList.size() - 1) / ENTRIES_PER_BLOCK) + 1;
    else
        numBlocksNeeded = 0;

    list<Entry> tempList(entryList);

    if (numBlocksNeeded > directorySize) {
        //add blocks to directory
        for (int i = 0; i < numBlocksNeeded - directorySize; i++)
            if(!directory.addBlock())
                return false;
    }
    else if(numBlocksNeeded < directorySize)
    {
        // remove uneeded blocks
        for(int i = directorySize; i > numBlocksNeeded; i--)
        {
            Block* blkPtr = directory.unlinkBlock();
            if(!freeList.addBlock(blkPtr))
            {
                delete blkPtr;
                return false;
            }
            delete blkPtr;
        }
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

    bool success = masterBlock.write(disk) && freeList.flush();
    pthread_mutex_unlock (&dirMutex);
    return success;
}

bool FileDirectory::addFile(string filename, int fcbNum) {
    if(filename.length() > MAX_NAME_SIZE - 1)
        filename = filename.substr(0, MAX_NAME_SIZE - 1);

    pthread_mutex_lock (&dirMutex);
    if(findEntry(filename) == NULL)
    {
        Entry newEntry(fcbNum, filename);
        entryList.push_back(newEntry);
        pthread_mutex_unlock (&dirMutex);
        return true;
    }
    else {
        pthread_mutex_unlock (&dirMutex);
        return false;
    }
}

int FileDirectory::findFile(string filename) {
    if(filename.length() > MAX_NAME_SIZE - 1)
        filename = filename.substr(0, MAX_NAME_SIZE - 1);

    pthread_mutex_lock (&dirMutex);
    Entry* entPtr = findEntry(filename);
    if(entPtr != NULL) {
        int f = entPtr->fcb;
        pthread_mutex_unlock (&dirMutex);
        return f;
    }
    else {
        pthread_mutex_unlock (&dirMutex);
        return -1; //file not found
    }
}

bool FileDirectory::renameFile(string filename, string newName) {
    if(filename.length() > MAX_NAME_SIZE - 1)
        filename = filename.substr(0, MAX_NAME_SIZE - 1);

    pthread_mutex_lock (&dirMutex);
    Entry* entPtr = findEntry(filename);
    if(entPtr != NULL)
    {
        entPtr->name = newName;
        pthread_mutex_unlock (&dirMutex);
        return true;
    }
    else {
        pthread_mutex_unlock (&dirMutex);
        return false; //file not found
    }

}

bool FileDirectory::removeFile(string filename) {
    if(filename.length() > MAX_NAME_SIZE - 1)
        filename = filename.substr(0, MAX_NAME_SIZE - 1);

    pthread_mutex_lock (&dirMutex);
    for (list<Entry>::iterator i = entryList.begin(); i != entryList.end(); i++) {
        if (!i->name.compare(filename)) {//returns 0 if strings are equal
            entryList.erase(i);
            pthread_mutex_unlock (&dirMutex);
            return true;
        }
    }
    pthread_mutex_unlock (&dirMutex);

    return false;
}

list<Entry> FileDirectory::listEntries() {
    return entryList;
}

Entry* FileDirectory::findEntry(string newName)
{
    for (list<Entry>::iterator i = entryList.begin(); i != entryList.end(); i++) {
        string temp = i->name;
        if (newName.compare(temp) == 0 ) {
            return &(*i);
        }
    }
    // Entry not found
    return NULL;
}