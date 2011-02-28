/* 
 * File:   Entry.h
 * Author: James Ekstrom, Kaleb Jungclaus, Matt Fischer
 *
 * Created on February 19, 2011, 5:08 PM
 */

#ifndef ENTRY_H
#define	ENTRY_H
#include <string>

using namespace std;

/**
 * This class represents an entry in a Directory. It is responsible for storing
 * the name and associated File Control Block number of a file.
 */
class Entry
{
public:

    /**
     * Creates a new directory entry with the given information.
     * @param fcbNum The FCB number of the file represented by this entry
     * @param filename The name of the file represented by this entry
     */
    Entry(int fcbNum,string filename) {
        fcb = fcbNum;
        name = filename;
    };

    /**
     * The File Control Block number of the file represented by this entry.
     */
    int fcb;

    /**
     * The name of the file represented by this entry.
     */
    string name;

};
#endif	/* ENTRY_H */

