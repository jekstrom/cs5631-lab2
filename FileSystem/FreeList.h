/* 
 * File:   FreeList.h
 * Author: james
 *
 * Created on February 2, 2011, 4:34 PM
 */

#ifndef FREELIST_H
#define	FREELIST_H

#include "BlockLinkedList.h"

class BlockGroup;

/**
 * Represents all of the currently unused blocks
 */
class FreeList : public BlockLinkedList
{
public:

    /**
     * The default number of blocks for a Disk to be used with this class.
     */
    const static int DEFAULT_NUMBER_OF_BLOCKS = 100;

    /**
     * This constructor generates a RAM object representing the free list for the Disk.
     * @param fileName 	is name of a file previously used to create a Disk
     * object. I.e., the simulated disk should exist before this call, but a Disk
     * object representing it should not yet be open.
     * @param createFreeList 	should be true if a free list has not yet been
     * created for this Disk. This will create the free list for this BlockGroup.
     * Otherwise, reads the starting and ending block numbers for the free list
     * from block 0. That is, set this parameter to true to initialize the free
     * list and loose all existing data, and to false to use the existing free list.
     */
    FreeList(char* fileName, bool createFreeList);

    /**
     * This constructor generates a RAM object representing the free list for the Disk.
     * @param disk A pointer to an open Disk object to be used by this free list.
     * @param createFreeList If true, a new freelist should be created on disk.
     * Else, an existing free list should be read from the disk.
     */
    FreeList(Disk* disk, bool createFreeList);

    /**
     * Writes out the current start block of the free list to block 0, and closes the Disk.
     * @return true iff the Free List could be closed.
     */
    bool close();

    /**
     * Spawn a new BlockGroup from the Free List. The new BlockGroup has one
     * block to start. This block is updated on disk by this call.
     * @return A new BlockGroup, or NULL if one could not be created.
     */
    BlockGroup* createNew();

    /**
     * Returns all blocks of the BlockLinkedList to the Free List.
     * @param bll After this call, this BlockLinkedList has been deallocated.
     */
    void returnBlocks(BlockLinkedList* bll);

    /**
     * Tests the functionality of this class
     */
    static void test();

private:

    /**
     * Block number used for the master block.
     */
    const static int MASTER_BLOCK_NUM = 0;
};

#endif	/* FREELIST_H */

