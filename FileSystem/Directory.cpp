//
#include "Directory.h"
#include "FreeList.h"
#include "BlockGroup.h"


Directory::Directory(Disk* disk, bool createNew) {
    FreeList *freeList = new FreeList(disk, false);
    Block *masterBlock = new Block(0, disk);

    if (createNew) {
        BlockGroup *directory = new BlockGroup(freeList);
        directory->addBlock();

        masterBlock->setPointer(3, directory->getStartBlockNumber());
        masterBlock->setPointer(4, directory->getEndBlockNumber());
        masterBlock->setPointer(5, directory->getNumberOfBlocks());

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
                            nameBuffer[i-sizeof(int)] = buffer[i];
                    }

                    Entry temp;
                    temp.fcb = (int) fcbBuffer;
                    temp.name = nameBuffer;

                    list.push_back(temp);

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

}

bool Directory::addFile(std::string filename, int fcbNum) {

}

int Directory::findFile(std::string filename) {

}

bool Directory::renameFile(std::string filename, std::string newName) {

}

bool Directory::removeFile(std::string filename) {

}

std::list<Entry> Directory::listEntries() {
    return list;
}