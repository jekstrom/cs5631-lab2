/* 
 * File:   File.h
 * Author: james
 *
 * Created on March 1, 2011, 7:14 PM
 */

#ifndef FILE_H
#define	FILE_H
#include "FileSystemHeaders.h"

class File {
public:
    /**
     * Reads a file from disk and opens it for either reading or writing.
     * @param filename The filename for the file to open
     * @param create If true, the file will be created if it doesn't exist, and
     * will open it for writing, ignoring readAccess
     * @param readAccess If true, opens the file for read access,celse opens
     * the file for write access
     * @param disk A pointer to the disk
     * @param directory A pointer to the directory
     */
    File(string filename, bool create, bool readAccess,
            Disk *disk, Directory *directory);

    /**
     * Destructor for the File class.
     */
    ~File()
    {
        delete currentBlockPtr;
    }

    /**
     * Opens a closed file for reading or writing.
     * @param readAccess If true, file will be opened for reading, else the file
     * will be opened for writing.
     * @return true iff file opened successfully
     */
    bool open(bool readAccess);

    /**
     * Closes current file access, can be reopened later.
     * @return true iff file closed successfully
     */
    bool close();

    /**
     * Deallocates the file, returning its blocks to the free list and
     * removing its entry from the directory.
     * @return true iff the file was deleted successfully
     */
    bool deleteFile();

    /**
     * Reads from the file.
     * @param buf The buffer to read bytes into
     * @param len The number of bytes to read
     * @return -1 if failure, 0 if end of file, else number of bytes read
     */
    int read(void* buf, int len);
    //{
        //if(!readOpen)
        //  return -1;
         
        //for(int bytesRead = 0; bytesRead < len; bytesRead++)
        //{
        //  if(atEOF)
        //      return bytesRead;
        //
        //  buf[bytesRead] = currentBlock[currentByte + sizeof(int)];
        //
        //  if(currentByte == fileBlocks.getBlockSize() - 1){
        //      fileBlocks.getNextBlock();
        //      currentBlock = fileBlocks.getCurrentBlock();
        //      currentByte = 0;
        //  }
        //  else
        //      currentByte++;
        //}
        
        //return len;
    //}
    

    /**
     * Writes to the file.
     * @param buf The buffer to write bytes from
     * @param len The number of bytes to write
     * @return -1 if failure, else number of bytes written
     */
    int write(void* buf, int len);
    //{
        //if(!writeOpen)
        //  return -1;
                         
        //for(int bytesWritten = 0; BytesWritten < len; bytesWritten++)
        //{
        //  if(currentByte == fileBlocks.getBlockSize() - 1){        
        //      currentBlock->write();
        //      if(!fileBlocks.add())
        //          return bytesWritten;
        //
        //      fileBlocks.getNextBlock();
        //      currentBlock = fileBlocks.getCurrentBlock();
        //      currentByte = 0;
        //  }
        //  else
        //      currentByte++;
        //
        //  currentBlock[currentByte + sizeof(int)] = buf[bytesWritten];
        //  endByte = currentByte;
        //}
        
        //currentBlock->write();
        //update fcb;
        //fcb.write();
        //return len;
    //}

    /**
     * A pointer to the directory where files should be retrieved from or
     * created in.
     */
    Directory *directoryPtr;

    /**
     * A pointer to the disk to read and write data from.
     */
    Disk *diskPtr;

    /**
     * The free list, as loaded from the disk.
     */
    FreeList freeList;

    /**
     * The name of the file.
     */
    string name;
    
    /**
     * The file control block for this file.
     */
    Block fcb;

    /**
     * The block group comprising this file.
     */
    BlockGroup fileBlocks;
    
    /**
     * Indicates whether or not the file is open for reading.
     */
    bool readOpen;

    /**
     * Indicates whether or not the file is open for writing.
     */
    bool writeOpen;

    /**
     * A pointer to the current block.
     */
    Block* currentBlockPtr;

    /**
     * The offset of the current byte within the current block.
     */
    unsigned int currentByte;
    
    /**
     * The block number of the last block in the file.
     */
    unsigned int endBlockNumber;
    
    /**
     * The offset of the final byte in the last block of the file.
     */
    unsigned int endByte;
    
private:
    
};


#endif	/* FILE_H */

