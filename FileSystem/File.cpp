// Kaleb Jungclaus, James Ekstrom, Matt Fischer

#include "FileSystemHeaders.h"

using namespace std;

File::File(string filename, bool create, bool readAccess, Disk* disk, Directory* directory) {
    directoryPtr = directory;
    diskPtr = disk;
    freeList = FreeList(diskPtr, false);
    name = filename;
    currentBlockPtr = NULL;
    readOpen = false;
    writeOpen = false;

    int fcbNum = directoryPtr->findFile(filename);

    if (-1 == fcbNum) {
        // File was not found
        if (!create)
            throw new FileNotFoundException;
        else {
            Block* blkPtr = freeList.unlinkBlock();
            if (blkPtr == NULL)
                throw new exception; // free list is empty
            fcb = *blkPtr;
            freeList.flush(); // save changes to free list

            directoryPtr->addFile(filename, fcb.getBlockNumber());
            // need to reload free list in case directory made changes to it
            freeList = FreeList(diskPtr, false);

            BlockGroup* bgPtr = freeList.createNew();
            if (bgPtr == NULL)
                throw new BlockGroupException; // BlockGroup could not be created
            fileBlocks = *bgPtr;
            endByte = 0;
            endBlockNumber = fileBlocks.getEndBlockNumber();

            // Update file control block
            fcb.setPointer(fileBlocks.getStartBlockNumber(), START_BLOCK_PTR_INDEX);
            fcb.setPointer(fileBlocks.getEndBlockNumber(), END_BLOCK_PTR_INDEX);
            fcb.setPointer(fileBlocks.getNumberOfBlocks(), NUM_BLOCKS_PTR_INDEX);
            fcb.setPointer(endByte, END_BYTE_PTR_INDEX);
            if (!fcb.write(diskPtr) || !freeList.flush())
                throw new exception;

            // open for writing
            open(false);
        }
    } else {
        // File exists, open it
        Block* fcbPtr = NULL;
        try {
            fcbPtr = new Block(fcbNum, diskPtr);
            fcb = *fcbPtr;
        } catch (CannotReadException e) {
            throw e;
        }

        fileBlocks = BlockGroup(fcb.getPointer(START_BLOCK_PTR_INDEX),
                fcb.getPointer(END_BLOCK_PTR_INDEX),
                fcb.getPointer(NUM_BLOCKS_PTR_INDEX), &freeList);
        endByte = fcb.getPointer(END_BYTE_PTR_INDEX);
        endBlockNumber = fileBlocks.getEndBlockNumber();

        // open file as indicated by readAccess
        open(readAccess);
        delete fcbPtr;
    }
}

File::~File() {
    if (currentBlockPtr != NULL)
        delete currentBlockPtr;
}

bool File::open(bool readAccess) {
    if (readOpen || writeOpen)
        return false; // file is already open

    if (readAccess) {
        // open for reading
        readOpen = true;
        // read from beginning
        fileBlocks.rewind();
        currentByte = 0;
    } else {
        // open for writing
        writeOpen = true;
        // write to end
        fileBlocks.fastForward();
        currentByte = endByte;
    }

    if (currentBlockPtr != NULL)
        delete currentBlockPtr;
    currentBlockPtr = fileBlocks.getCurrentBlock();
    return true;
}

bool File::close() {
    readOpen = false;
    writeOpen = false;
    delete currentBlockPtr;
    currentBlockPtr = NULL;
    return true;
}

bool File::deleteFile() {
    // remove file from directory
    directoryPtr->removeFile(name);

    // return blocks, update free list
    freeList = FreeList(diskPtr, false);
    freeList.addBlock(&fcb);
    freeList.returnBlocks(&fileBlocks);

    return freeList.flush();
}

int File::read(void* buf, int len) {
    if (!readOpen)
        return -1;

    char* readBuf = (char*) buf;
    int lastByteIndex = fileBlocks.getBlockSize() - 1;
    unsigned char* blockBuf = currentBlockPtr->getBuffer();
    int currentBlockNumber = currentBlockPtr->getBlockNumber();
    int fcbNum = fcb.getBlockNumber();
    if (fcbNum == currentBlockNumber) {
        fileBlocks.getNextBlock();
        currentBlockPtr = fileBlocks.getCurrentBlock();
        blockBuf = currentBlockPtr->getBuffer();
    }
    for (int bytesRead = 0; bytesRead < len; bytesRead++) {
        if (currentBlockPtr == NULL ||
                (endBlockNumber == currentBlockNumber && currentByte > endByte)) {
            return bytesRead - 1;
        }

        readBuf[bytesRead] = blockBuf[currentByte + sizeof (int) + 1];

        //check null-termination
        if (readBuf[bytesRead] == 0 && endBlockNumber == currentBlockNumber)
            return bytesRead;

        if (currentByte == lastByteIndex) {
            // advance to next block
            fileBlocks.getNextBlock();
            delete currentBlockPtr;
            currentBlockPtr = fileBlocks.getCurrentBlock();
            currentByte = 0;

            if (currentBlockPtr != NULL) {
                blockBuf = currentBlockPtr->getBuffer();
                currentBlockNumber = currentBlockPtr->getBlockNumber();
            }
        } else
            currentByte++;
    }

    // Have read the desired amount
    return len;
}

int File::write(const void* buf, int len) {
    if (!writeOpen)
        return -1;

    char* writeBuf = (char*) buf;
    int lastByteIndex = fileBlocks.getBlockSize() - 1;
    int written = -1;
    currentBlockPtr = fileBlocks.getCurrentBlock();
    unsigned char* blockBuf = currentBlockPtr->getBuffer();
//    int fcbNum = fcb.getBlockNumber();
//    int currentBlockNum = currentBlockPtr->getBlockNumber();
//    if (fcbNum == currentBlockNum) {
//        freeList = FreeList(diskPtr, false);
//        fileBlocks.addBlock();
//        fileBlocks.getNextBlock();
//        currentBlockPtr = fileBlocks.getCurrentBlock();
//        blockBuf = currentBlockPtr->getBuffer();
//    }

    for (int bytesWritten = 0; bytesWritten < len; bytesWritten++) {
        if (currentByte == lastByteIndex) {

            //write out changes to disk
            if (!fcb.write(diskPtr) || !currentBlockPtr->write(diskPtr) || !freeList.flush())
                return -1; // error has occurred

            freeList = FreeList(diskPtr, false); // update free list

            if (!fileBlocks.addBlock()) {
                written = bytesWritten; // couldn't extend file, stop writing
                break;
            }

            delete currentBlockPtr;
            if (!fileBlocks.getNextBlock())
                return -1;
            currentBlockPtr = fileBlocks.getCurrentBlock();
            currentByte = 0;
            endBlockNumber = fileBlocks.getEndBlockNumber();
            blockBuf = currentBlockPtr->getBuffer();
        } else
            currentByte++;

        blockBuf[currentByte + sizeof (int) ] = writeBuf[bytesWritten];
        endByte = currentByte;
    }

    if (written < 0)
        written = len;

    // write changes to disk
    fcb.setPointer(fileBlocks.getStartBlockNumber(), START_BLOCK_PTR_INDEX);
    fcb.setPointer(fileBlocks.getEndBlockNumber(), END_BLOCK_PTR_INDEX);
    fcb.setPointer(fileBlocks.getNumberOfBlocks(), NUM_BLOCKS_PTR_INDEX);
    fcb.setPointer(endByte, END_BYTE_PTR_INDEX);

    if (!fcb.write(diskPtr) || !currentBlockPtr->write(diskPtr) || !freeList.flush())
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
