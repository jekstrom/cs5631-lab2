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
#include <string>
#include <list>

/**
 * This class represents a (and for now, the only) directory in the file system.
 * It is responsible for relating human-readable file names to the appropriate
 * File Contol Blocks on disk.
 */
class Directory
{
public:
    const static int ENTRY_SIZE = 36;
    const static int MAX_NAME_SIZE = 32;
    const static int ENTRIES_PER_BLOCK = 14;

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
     * @param filename name for the new file
     * @param fcbNum File Control Block number for the new file
     * @return true iff the file was successfully added
     */
    bool addFile(std::string filename, int fcbNum);

    /**
     * Retrieves the FCB number of a file with the giben name
     * @param filename the name of the file to search for
     * @return the File Control Block number of the file, or -1 if the file wasn't found
     */
    int findFile(std::string filename);

    /**
     * Changes the name of a file in the directory.
     * @param filename The current name of the file
     * @param newName The new name for the file
     * @return true iff the file was found
     */
    bool renameFile(std::string filename, std::string newName);

    /**
     * Removes a file from the directory.
     * @param filename The name of the file to remove.
     * @return true iff the file was found and removed successfully
     */
    bool removeFile(std::string filename);

    /**
     * Returns a list of all entries in the directory.
     * @return A list containing every entry in the directory
     */
    std::list<Entry> listEntries();


private:
    std::list<Entry> entryList;

    Disk* disk;
};

#endif	/* DIRECTORY_H */

