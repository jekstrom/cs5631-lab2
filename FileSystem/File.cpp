// Kaleb Jungclaus, James Ekstrom, Matt Fischer

#include "FileSystemHeaders.h"

using namespace std;

File::File(string filename, bool create, bool readAccess, Disk* disk, Directory* directory)
{
    directoryPtr = directory;
    diskPtr = disk;
    freeList = FreeList(diskPtr, false);
    name = filename;
    currentBlockPtr = NULL;

    int fcbNum = directoryPtr->findFile(filename);

    if(-1 == fcbNum)
    {
        // File was not found
        if(!create)
            throw new FileNotFoundException;
        else
        {
            Block* blkPtr = freeList.unlinkBlock();
            if(blkPtr == NULL)
                throw new exception;// free list is empty
            fcb = *blkPtr;

            directoryPtr->addFile(filename, fcb.getBlockNumber());
            // need to reload free list in case directory made changes to it
            freeList = FreeList(diskPtr, false);

            BlockGroup* bgPtr = freeList.createNew();
            if(bgPtr == NULL)
                throw new BlockGroupException; // BlockGroup could not be created
            fileBlocks = *bgPtr;
            endByte = 0;
            endBlockNumber = fileBlocks.getEndBlockNumber();

            // Update file control block
            fcb.setPointer(START_BLOCK_PTR_INDEX, fileBlocks.getStartBlockNumber());
            fcb.setPointer(END_BLOCK_PTR_INDEX, fileBlocks.getEndBlockNumber());
            fcb.setPointer(NUM_BLOCKS_PTR_INDEX, fileBlocks.getNumberOfBlocks());
            fcb.setPointer(END_BYTE_PTR_INDEX, endByte);
            if(!fcb.write(diskPtr) || !freeList.flush())
                throw new exception;

            // open for writing
            open(readAccess);
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

        fileBlocks = BlockGroup(fcb.getPointer(START_BLOCK_PTR_INDEX),
                   fcb.getPointer(END_BLOCK_PTR_INDEX),
                   fcb.getPointer(NUM_BLOCKS_PTR_INDEX), &freeList);
        endByte = fcb.getPointer(END_BYTE_PTR_INDEX);
        endBlockNumber = fileBlocks.getEndBlockNumber();

        // open file as indicated by readAccess
        open(readAccess);
    }
}

File::~File() {
    delete currentBlockPtr;
}

bool File::open(bool readAccess)
{
    if(readOpen || writeOpen)
        return false; // file is already open
    
    if(readAccess)
    {
        // open for reading
        readOpen = true;
        // read from beginning
        fileBlocks.rewind();
        currentByte = 0;
    }
    else
    {
        // open for writing
        writeOpen = true;
        // write to end
        fileBlocks.fastForward();
        currentByte = endByte;
    }

//    delete currentBlockPtr;
    currentBlockPtr = fileBlocks.getCurrentBlock();
    return true;
}

bool File::close()
{
    readOpen = false;
    writeOpen = false;
    return true;
}

bool File::deleteFile()
{
    // remove file from directory
    directoryPtr->removeFile(name);

    // return blocks, update free list
    freeList = FreeList(diskPtr, false);
    freeList.addBlock(&fcb);
    freeList.returnBlocks(&fileBlocks);

    return freeList.flush();
}

int File::read(void* buf, int len)
{
    if(!readOpen)
        return -1;

    char* readBuf = (char*) buf;
    int lastByteIndex = fileBlocks.getBlockSize() - 1;
    unsigned char* blockBuf = currentBlockPtr->getBuffer();
    int currentBlockNumber = currentBlockPtr->getBlockNumber();
    for(int bytesRead = 0; bytesRead < len + 1; bytesRead++)
    {
        if(currentBlockPtr == NULL ||
                (endBlockNumber == currentBlockNumber && currentByte > endByte)) {
            buf = readBuf;
            return bytesRead;
        }

        readBuf[bytesRead] = blockBuf[currentByte + sizeof(int) + 1];

        if(currentByte == lastByteIndex)
        {
            // advance to next block
            fileBlocks.getNextBlock();
            delete currentBlockPtr;
            currentBlockPtr = fileBlocks.getCurrentBlock();
            currentByte = 0;

            if(currentBlockPtr != NULL)
            {
                blockBuf = currentBlockPtr->getBuffer();
                currentBlockNumber = currentBlockPtr->getBlockNumber();
            }
        }
        else
            currentByte++;
    }

    buf = readBuf;
    // Have read the desired amount
    return len;
}

int File::write(const void* buf, int len)
{
    if(!writeOpen)
        return -1;

    char* writeBuf = (char*) buf;
    int lastByteIndex = fileBlocks.getBlockSize() - 1;
    int written = -1;
    unsigned char* blockBuf = currentBlockPtr->getBuffer();

    for(int bytesWritten = 0; bytesWritten < len; bytesWritten++)
    {
        if(currentByte == lastByteIndex)
        {
            freeList = FreeList(diskPtr, false); // update free list

            if(!currentBlockPtr->write(diskPtr))
                return -1; // error
            if(!fileBlocks.addBlock())
            {
                written = bytesWritten; // couldn't extend file, stop writing
                break;
            }

            delete currentBlockPtr;
            fileBlocks.getNextBlock();
            currentBlockPtr = fileBlocks.getCurrentBlock();
            currentByte = 0;
            endBlockNumber = fileBlocks.getEndBlockNumber();

            blockBuf = currentBlockPtr->getBuffer();
        }
        else
            currentByte++;

        blockBuf[currentByte + sizeof(int)] = writeBuf[bytesWritten];

        endByte = currentByte;
    }

    if(written < 0)
        written = len;

    // write changes to disk
    fcb.setPointer(START_BLOCK_PTR_INDEX, fileBlocks.getStartBlockNumber());
    fcb.setPointer(END_BLOCK_PTR_INDEX, fileBlocks.getEndBlockNumber());
    fcb.setPointer(NUM_BLOCKS_PTR_INDEX, fileBlocks.getNumberOfBlocks());
    fcb.setPointer(END_BYTE_PTR_INDEX, endByte);

    if(!fcb.write(diskPtr) || !currentBlockPtr->write(diskPtr) || !freeList.flush())
        return -1; // error has occurred

    // Have written the desired amount
    return written;
}

int File::getFcbNumber() {
    return fcb.getBlockNumber();
}

string File::getName() {
    return name;
}

bool File::isOpenForRead() {
    return readOpen;
}