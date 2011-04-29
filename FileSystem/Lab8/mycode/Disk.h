/* 
 * File:   Disk.h
 * Author: chris
 *
 * Created on January 25, 2009, 1:18 PM
 */

#ifndef _DISK_H
#define	_DISK_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "BadNumberOfBlocksException.h"
#include "CannotReadException.h"

/**
 * Provides access to fixed-length blocks stored on a simulated
 * a disk device using a file.
 *
 * @author chris
 */
class Disk {
public:
    /** The default size of disk blocks. Use this, for example, when calling the
     *  Disk constructor.
     */
    static const int DEFAULT_BLOCK_SIZE = 512; // bytes

    /**
     * @return the number of blocks on the Disk, as established via the constructor.
     */
    int numberOfBlocks() {
        return m_numberOfBlocks;
    }

    /**
     * @return the block size for the Disk, as established via the constructor.
     */
    int blockSize() {
        return m_blockSize;
    }

    /** Open the Disk.
     * @param deviceName should refer to the file name for the simulated disk that
     * will be used to store the disk blocks. E.g., "disk1" for a file name.
     * Does not format or otherwise alter the simulated disk (i.e., file).
     * The disk (or file) itself does not record the number of blocks on the disk (or file)
     * nor does it record the block size,
     * so this info needs to be provided each time a Disk is opened.
     *
     * The file is created if it does not exist. If you are reducing the number of blocks
     * in a pre-existing file, you will want to delete the file first.
     */
    Disk(char *deviceName, int numberOfBlocks, int blockSize) {
        pthread_mutex_init(&diskMutex, NULL);
        m_debugInfo = false;
        m_deviceName = deviceName;
        m_numberOfBlocks = numberOfBlocks;
        m_blockSize = blockSize;

        if ((numberOfBlocks <= 0) || (blockSize <= 0)) {
            printf("Disk.Disk: ERROR: bad number of blocks or block size\n");
            throw new BadNumberOfBlocksException();
        }

        m_fd = open(deviceName, O_RDWR | O_CREAT, S_IRWXU);
        if (-1 == m_fd) {
            printf("Disk.Disk: ERROR: Cannot open file for disk: %s\n", deviceName);
            throw new CannotReadException();
        }
    }
    
    /**
     * Change the debug state. If debug state is false, debug info is not printed.
     * The debug state defaults to true.
     */
    void Debug(bool state) {
        m_debugInfo = state;
    }

    /**
      * Given that a Disk is open, "low-level" format it.
      * You will loose all data on the device (or file),
      * be careful! This class does not keep track of the
      * number of blocks nor the block size of the disk
      * in block 0 of the disk (nor anywhere else on the disk).
      *
      * (Rationale: Particularly, for the block size, in order for us to
      * read a block from the disk (e.g., block 0), we need the block size. If the block
      * size is on disk, and we need that to read a block, how do we know the block size
      * to read block 0, which contains the block size?)
      *
      * Return true iff the simulated disk could be formatted.
      */
    bool Format() {
        pthread_mutex_lock (&diskMutex);
        // Format our file (simulated) device by writing out empty blocks.

        // First need to seek to the start.
        if (lseek(m_fd, 0, SEEK_SET) == -1) {
            printf("Disk.Format: ERROR: Could not seek into device.\n");
            pthread_mutex_unlock (&diskMutex);
            return false;
        }
        
        // Create an empty block of unsigned chars (bytes)
        unsigned char* block = new unsigned char[m_blockSize];
        for (int i=0; i < m_blockSize; i++) {
            block[i] = 0; // initialize the block with 0 values
        }

        // Write the empty blocks out
        for (int i=0; i < m_numberOfBlocks; i++) {
	    // write the block
            if (write(m_fd, (void*) block, m_blockSize) != m_blockSize) { 
                delete [] block;
                printf("Disk.Format: ERROR: Could not write block to file\n");
                printf("Disk.Format: ERROR: Problem writing block number: %d\n", i);
                pthread_mutex_unlock (&diskMutex);
                return false;
            }
        }

        // All is well-- we got through the disk formatting steps.
        printf("Disk.Format: SUCCESS: Formatted disk with %d", m_numberOfBlocks);
        printf(" blocks, each of size = %d bytes.\n", m_blockSize);
        delete [] block;
        pthread_mutex_unlock (&diskMutex);
        return true;
    }

    /** Close the disk.
     * Returns true if the file was closed successfully.
     */
    bool Close() {
        if (close(m_fd) == -1) {
            printf("Disk.Close: ERROR: Could not close file\n");
            return false;
        }

        printf("Disk.Close: SUCCESS: Closed disk!\n");

        return true;
    }

    /** Write out a block of data to the given block number on the disk. 
      * @param blockNumber needs to be within range for this disk
      * (block numbers start at 0, and end at numberOfBlocks - 1).
      * @param block needs to have length (number of bytes)
      * the same as the block size for this disk.
      * @return true iff block write succeeded.
      */
    bool WriteBlock(int blockNumber, unsigned char* block) {
        pthread_mutex_lock (&diskMutex);

        if ((blockNumber < 0) || (blockNumber >= m_numberOfBlocks)) {
            printf("Disk.WriteBlock: ERROR: block number incorrect\n");
            pthread_mutex_unlock (&diskMutex);
            return false;
        }

        // Compute the byte offset into the device for the block we are going to write.
        int offset = blockNumber * m_blockSize;

        // Seek into the device
        if (lseek(m_fd, offset, SEEK_SET) == -1) {
            printf("Disk.WriteBlock: ERROR: Could not seek into device.\n");
            pthread_mutex_unlock (&diskMutex);
            return false;
        }

        if (write(m_fd, (void *) block, m_blockSize) != m_blockSize) {
            printf("Disk.WriteBlock: ERROR: block write failed\n");
            pthread_mutex_unlock (&diskMutex);
            return false;
        }

        if (m_debugInfo) {
           printf("Disk.WriteBlock: SUCCESS: Wrote disk block to disk.\n");
        }

        pthread_mutex_unlock (&diskMutex);
        return true;
    }

    /** Read a block of data from the given block number on the disk.
      * @param blockNumber needs to be within range for this disk
      * (block numbers start at 0, and end at numberOfBlocks - 1).
      * @param block needs to have length (in bytes)
      * the same as the block size for this disk.
      * This must be previously allocated by the caller.
      * @return true iff block read succeeded.
      * The block of data in the parameter (block) passed may or may not
      * have been modified if this call returns false.
      */
    bool ReadBlock(int blockNumber, unsigned char *block) {
        pthread_mutex_lock (&diskMutex);

        if ((blockNumber < 0) || (blockNumber >= m_numberOfBlocks)) {
            printf("Disk.ReadBlock: ERROR: block number incorrect\n");
            pthread_mutex_unlock (&diskMutex);
            return false;
        }

        // Compute the offset into the file for the block we are going to read.
        long offset = blockNumber * m_blockSize;

        if (m_debugInfo) {
            printf("Disk.ReadBlock: offset= %d\n", offset);
        }

        // Seek into the device
        if (lseek(m_fd, offset, SEEK_SET) == -1) {
            printf("Disk.ReadBlock: ERROR: Could not seek into device.\n");
            pthread_mutex_unlock (&diskMutex);
            return false;
        }

        //printf("Here1"); fflush(stdout);

        int result = read(m_fd, (void *) block, m_blockSize);
        if (result != m_blockSize) {
            printf("Disk.ReadBlock: ERROR: block read failed; read result= %d\n", result);
            printf("Disk.ReadBlock: ERROR: attempting to read number of bytes= %d\n", m_blockSize);
            pthread_mutex_unlock (&diskMutex);
            return false;
        }

        //printf("Here2"); fflush(stdout);

        if (m_debugInfo) {
            printf("Disk.ReadBlock: read %d bytes\n.", m_blockSize);
        }

        pthread_mutex_unlock (&diskMutex);
        return true;
    }

    static void Test() {
        char *s = NULL;
        char *deviceName = "disk1";
        int numberOfBlocks = 1000;
        int cmd = 0;
        Disk *disk1 = NULL;
        int blockNumber = 0;
        int byteToWrite;
        unsigned char *block = new unsigned char[Disk::DEFAULT_BLOCK_SIZE];

        for (;;) {
            printf("\nmain: Enter command:\n");
            printf("   0) Exit program\n");
            printf("   1) Open disk object-- named: %s\n", deviceName);
            printf("   2) Reformat disk-- named: %s\n", deviceName);
            printf("   3) Close disk-- named: %s (keep contents)\n", deviceName);
            printf("   4) Write disk block to open disk\n");
            printf("   5) Read disk block from open disk\n");

            printf("Enter command: ");
            scanf("%d", &cmd);

            switch (cmd) {
                case 0:
                    return;

                case 1: // create and open disk1
                    if (disk1 != NULL) {
                        printf("main: ERROR: disk1 is open!n");
                        break;
                    }

                    disk1 = new Disk(deviceName, numberOfBlocks, Disk::DEFAULT_BLOCK_SIZE);
                    printf("main: SUCCESS: disk1 is now open.");
                    break;

                case 2: // reformat disk1
                    if (disk1 == NULL) {
                        printf("main: ERROR: disk1 is not open!\n");
                        break;
                    }

                    if (disk1->Format()) {
                        printf("main: SUCCESS: formatted disk1!\n");
                    } else {
                        printf("main: ERROR: could not format disk1!\n");
                    }
                    break;

                case 3: // close disk1 (don't reformat)
                    if (disk1 != NULL) {
                        if (disk1->Close()) {
                            printf("main: SUCCESS: closed disk1!\n");
                        } else {
                            printf("main: ERROR: could not close disk1!\n");
                        }
                    } else {
                        printf("main: ERROR: disk not open!\n");
                        break;
                    }

                    disk1 = NULL;
                    printf("main: SUCCESS: disk1 closed\n");
                    break;

                case 4: // write disk block to open disk
                    if (disk1 == NULL) {
                        printf("main: ERROR: disk1 is not open!\n");
                        break;
                    }

                    printf("Enter block number to write: ");
                    scanf("%d", &blockNumber);

                    printf("Enter integer value to write, as a test, to all bytes of block: ");
                    scanf("%d", &byteToWrite);

                    printf("main: Attempting to write block %d\n", blockNumber);

                    for (int i=0; i < Disk::DEFAULT_BLOCK_SIZE; i++) {
                        block[i] = byteToWrite;
                    }

                    if (! disk1->WriteBlock(blockNumber, block)) {
                        printf("main: ERROR: Block write failed!\n");
                    } else {
                        printf("main: SUCCESS: Block written!\n");
                    }
                    break;

                case 5: // read disk block from open disk
                    if (disk1 == NULL) {
                        printf("main: ERROR: disk1 is not open!\n");
                        break;
                    }

                    printf("Enter block number to read: ");
                    scanf("%d", &blockNumber);

                    printf("main: Attempting to read block %d\n", blockNumber);

                    if (! disk1->ReadBlock(blockNumber, block)) {
                        printf("main: ERROR: Block read failed!\n");
                    } else {
                        printf("main: SUCCESS: Block read!\n");
                        for (int i=0; i < Disk::DEFAULT_BLOCK_SIZE; i++) {
                            printf("%d,", block[i]);
                        }
                        printf("\n");
                    }
                    break;

                default:
                    printf("main: Bad command!\n");
                    break;
            } // end switch
        } // end for
    } // end Test()

private:
    int m_fd; // file descriptor
    char *m_deviceName;
    int m_numberOfBlocks;
    int m_blockSize;

    // whether or not to print out debugging info
    bool m_debugInfo;

    pthread_mutex_t diskMutex;
 };





#endif	/* _DISK_H */

