// Kaleb Jungclaus, James Ekstrom, Matt Fischer

#include "FileSystemHeaders.h"

using namespace std;

File::File(string filename, bool create, bool readAccess, Disk* disk, Directory* directory)
{
    directoryPtr = directory;
    diskPtr = disk;
    freeList = FreeList(diskPtr, false);

    int fcbNum = directoryPtr->findFile(filename);

    if(-1 == fcbNum)
    {
        // File was not found
        if(!create)
            throw FileNotFoundException;
        else
        {
            Block* blkPtr = freeList.unlinkBlock();
            if(blkPtr == NULL)
                throw exception;// free list is empty
            fcb = *blkPtr;

            directoryPtr->addFile(filename, fcb.ge newtBlockNumber());
            // need to reload free list in case directory made changes to it
            freeList = FreeList(diskPtr, false);

            BlockGroup* bgPtr = freeList.createNew();
            if(bgPtr == NULL)
                throw exception; // BlockGroup could not be created
            fileBlocks = *bgptr;
            if(!fileBlocks.addBlock())
                throw exception;
            endByte = 0;
            endBlockNumber = fileBlocks.getEndBlockNumber();

            // Update file control block
            fcb.setPointer(0, fileBlocks.getStartBlockNumber());
            fcb.setPointer(1, fileBlocks.getEndBlockNumber());
            fcb.setPointer(2, fileBlocks.getNumberOfBlocks());
            fcb.setPointer(3, endByte);
            if(!fcb.write())
                throw exception;

            // open for writing
            open(false);
        }
    }
    else
    {
        // File exists, open it
        try
        {
            fcb = Block(fcbNum, diskPtr);
        }
        catch(CannotReadException e)
        {
            throw e;
        }

        fileBlocks = BlockGroup(fcb.getPointer(0), fcb.getPointer(1),
                   fcb.getPointer(2), &freeList);
        endByte = fcb.getPointer(3);
        endBlockNumber = fileBlocks.getEndBlockNumber();

        open(readAccess);
    }
}
