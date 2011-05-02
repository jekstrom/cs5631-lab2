/* 
 * File:   GlobalFileTable.h
 * Author: kaleb
 *
 * Created on April 26, 2011, 7:07 PM
 */

#ifndef GLOBALFILETABLE_H
#define	GLOBALFILETABLE_H

#include <pthread.h>
#include <list>
#include <iterator>

using namespace std;

class GlobalFileTable
{
public:

    /**
     * Constructs a new global file table.
     */
    GlobalFileTable()
    {
        pthread_mutex_init(&tableMutex, NULL);
        pthread_mutex_init(&deletionMutex, NULL);
    }

    /**
     * Adds a reference to an open file, creating a new entry if one does not
     * already exist.
     * @param fcbNum The FCB number of the file to add a reference for
     */
    void addReference(int fcbNum)
    {
        pthread_mutex_lock(&tableMutex);

        // search table to see if file is already open
        bool found = false;
        for(list<tableEntry>::iterator it = entryList.begin(); it != entryList.end(); it++)
            if(it->fcb == fcbNum)
            {
                // file is already open, increment reference count
                it->refCount++;
                found = true;
                break;
            }

        if(!found)
        {
            // file is not open, create a new entry
            tableEntry newEntry;
            newEntry.fcb = fcbNum;
            newEntry.refCount = 1;
            pthread_mutex_init(&newEntry.fileMutex, NULL);

            entryList.push_back(newEntry);
        }

        pthread_mutex_unlock(&tableMutex);
    }

    /**
     * Removes a reference to an open file. Will remove an entry entirely if its
     * reference count reaches xero, destroying the corresponding mutex.
     * @param fcbNum The FCB number of the file to remove a reference to
     */
    void removeReference(int fcbNum)
    {        
        pthread_mutex_lock(&tableMutex);

        // search table
        for(list<tableEntry>::iterator it = entryList.begin(); it != entryList.end(); it++)
            if(it->fcb == fcbNum)
            {
                // decrement reference count
                it->refCount--;

                // if there are no more references, remove entry
                if(it->refCount <= 0)
                {
                    pthread_mutex_destroy(&it->fileMutex);
                    entryList.erase(it);
                }

                break;
            }

        pthread_mutex_unlock(&tableMutex);
    }

    /**
     * Gets a pointer to the mutex corresponding to a file in the table, which
     * can be used to synchronize actions on the file. NULL will be returned if
     * the file is not found in the table.
     * @param fcbNum The FCB number of the file to get a mutex for
     * @return A pointer to a mutex (which should not be deleted), or NULL
     */
    pthread_mutex_t* getMutex(int fcbNum)
    {
        // search table
        for(list<tableEntry>::iterator it = entryList.begin(); it != entryList.end(); it++)
            if(it->fcb == fcbNum)
                return &(it->fileMutex);

        // file wasn't in table
        return NULL;
    }

    /**
     * Retrieves a pointer to a mutex which can be used to ensure deletion of a
     * file is not interrupted by another thread.
     * @return A pointer to a mutex
     */
    pthread_mutex_t* getDeletionMutex()
    {
        return &deletionMutex;
    }

private:

    struct tableEntry
    {
        int fcb;

        pthread_mutex_t fileMutex;

        int refCount;
    };

    list<tableEntry> entryList;

    pthread_mutex_t tableMutex;

    pthread_mutex_t deletionMutex;
};

#endif	/* GLOBALFILETABLE_H */

