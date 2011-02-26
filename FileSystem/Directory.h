/* 
 * File:   Directory.h
 * Author: kaleb
 *
 * Created on February 17, 2011, 7:03 PM
 */

#ifndef DIRECTORY_H
#define	DIRECTORY_H

#include "Disk.h"
#include "Entry.h"
#include "BlockGroup.h"
#include <string>
#include <list>

using namespace std;

/**
 * This class represents a (and for now, the only) directory in the file system.
 * It is responsible for relating human-readable file names to the appropriate
 * File Contol Blocks on disk.
 */
class Directory
{
public:

    /**
     * Creates a new directory on disk, or loads an existing directory from a disk,
     * based on the value of createNew.
     * @param disk The disk to be used by this directory
     * @param createNew true to create a new directory, false to load an existing one
     */
    Directory(Disk* disk, bool createNew);

    /**
     * Saves the directory to disk.
     * @return true iff the directory flush was successful
     */
    bool flush();

    /**
     * Add a file with the given name and FCB number to the directory.
     * @param filename Name for the new file, will be truncated if longer than
     * 31 characters
     * @param fcbNum File Control Block number for the new file
     * @return true iff the file was successfully added
     */
    bool addFile(string filename, int fcbNum);

    /**
     * Retrieves the FCB number of a file with the given name.
     * @param filename the name of the file to search for, will be truncated if
     * longer than 31 characters
     * @return the File Control Block number of the file, or -1 if the file wasn't found
     */
    int findFile(string filename);

    /**
     * Changes the name of a file in the directory.
     * @param filename The current name of the file, will be truncated if longer
     * than 31 characters
     * @param newName The new name for the file, will be truncated if longer than
     * 31 characters
     * @return true iff the file was found
     */
    bool renameFile(string filename, string newName);

    /**
     * Removes a file from the directory.
     * @param filename The name of the file to remove, will be truncated if longer
     * than 31 characters
     * @return true iff the file was found and removed successfully
     */
    bool removeFile(string filename);

    /**
     * Returns a list of all entries in the directory.
     * @return A list containing every entry in the directory
     */
    list<Entry> listEntries();


private:
    list<Entry> entryList;

    Disk* disk;

    BlockGroup directory;

    FreeList freeList;
    /**
     * The number of bytes in a directory entry.
     */
    const static int ENTRY_SIZE = 36;
    
    /**
     * The number of bytes in each entry that can be used for the filename. Thus,
     * the effective maximum filename length is 31.
     */
    const static int MAX_NAME_SIZE = 32;

    /**
     * The number of directory entries per disc block.
     */
    const static int ENTRIES_PER_BLOCK = 14;

    /**
     * Retrieves a pointer to the entry with the specified name.
     * @param name The name of the entry to search for
     * @return A pointer to the specified entry, or NULL if the entry wasn't found
     */
    Entry* findEntry(string name);
};

#endif	/* DIRECTORY_H */

