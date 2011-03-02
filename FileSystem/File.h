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
     * File constructor constructs or opens a file from disk
     * @param filename the filename for the file
     * @param create if true creates a file if it doesn't exist
     * @param readAccess if true, opens the file for read access,
     * else it's open for write access
     * @param freelist the freelist on disk
     * @param directory the directory on disk
     */
    File::File(string filename, bool create, bool readAccess,
            Disk *disk, Directory *directory) {
    }

    /**
     * Opens a closed file for reading or writing
     * @param readAccess true if opening for read access, else it is write access
     * @return true iff file opened successfully
     */
    bool File::open(bool readAccess) {}

    /**
     * closes current open file, can be opened later for reading and writing
     * @return true iff file closed successfully
     */
    bool File::close() {}

    /**
     * Reads an open file
     * @param buf the buffer to read bytes into
     * @param len the number of bytes to read
     * @return -1 if failure, 0 if end of file, else number of bytes read
     */
    int File::read(void* buf, int len) {}

    /**
     * Writes an open file
     * @param buf the buffer to write bytes from
     * @param len the number of bytes to write
     * @return -1 if failure, 0 if end of file, else number of bytes written
     */
    int File::write(void* buf, int len) {}

    Directory *directory;
    Disk *disk;
    BlockGroup fileBlocks;
    unsigned int blockNumber;
    unsigned int fp;
};


#endif	/* FILE_H */

