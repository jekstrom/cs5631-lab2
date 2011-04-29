/* 
 * File:   OpenFileTable.h
 * Author: kaleb
 *
 * Created on April 21, 2011, 7:28 PM
 */

#ifndef OPENFILETABLE_H
#define	OPENFILETABLE_H

#include <string>
#include <list>
#include <iterator>
#include "FileSystemHeaders.h"

using namespace std;

class OpenFileTable {
public:

    /**
     * Creates and initailizes a new open file table.
     */
    OpenFileTable()
    {
        fileCount = 0;
    }

    /**
     * Makes a new entry in the open file table and returns a file descriptor.
     * @param filePtr A pointer to the File to make an entry for
     * @return The file descriptor for the new entry
     */
    int addEntry(File* filePtr)
    {
        tableEntry entry;
        entry.fileDescriptor = fileCount;
        entry.filePtr = filePtr;

        table.push_back(entry);
        fileCount++;
        return entry.fileDescriptor;
    }

    /**
     * Retrieves a File pointer corresponding to a file descriptor.
     * @param fd A file descriptor
     * @return A File pointer corresponding to the file descriptor, or
     *         NULL if the file descriptor is not in the table.
     */
    File* getFilePtr(int fd)
    {
        for(list<tableEntry>::iterator it = table.begin(); it != table.end(); it++)
        {
            if(it->fileDescriptor == fd)
                return it->filePtr;
        }

        //file not found
        return NULL;
    }

    /**
     * Searches the open file table for a file with a particular name, and returns
     * its file descriptor.
     * @param filename THe name of a file to search for
     * @return The file descriptor of the file if it is found, -1 otherwise
     */
    int getFD(string filename)
    {
        for(list<tableEntry>::iterator it = table.begin(); it != table.end(); it++)
        {
            if(it->filePtr->getName() == filename)
                return it->fileDescriptor;
        }
    }

    /**
     * Takes a file descriptor and removes its corresponding entry from the table.
     * This closes the corresponding file.
     * @param fd A file descriptor
     */
    void removeEntry(int fd)
    {        
        for(list<tableEntry>::iterator it = table.begin(); it != table.end(); it++)
        {
            if(it->fileDescriptor == fd)
            {
                it->filePtr->close();
                table.erase(it);
                break;
            }
        }
    }

private:

    struct tableEntry
    {
        int fileDescriptor;
        File* filePtr;
    };

    int fileCount;

    list<tableEntry> table;
};

#endif	/* OPENFILETABLE_H */

