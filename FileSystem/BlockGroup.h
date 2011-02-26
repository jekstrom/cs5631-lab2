/* 
 * File:   BlockGroup.h
 * Author: james
 *
 * Created on February 2, 2011, 4:34 PM
 */

#ifndef BLOCKGROUP_H
#define	BLOCKGROUP_H

#include "BlockLinkedList.h"
#include "FreeList.h"

/**
     * Creates a new BlockGroup.
     * @param bll BlockLinkedList to use to initialize.
     */
class BlockGroup : public BlockLinkedList
{
public:

    /**
     * Creates a new BlockGroup.
     * @param bll BlockLinkedList to use to initialize.
     */
    BlockGroup(BlockLinkedList bll);

    /**
     * Creates a new BlockGroup which will get new nodes from the given FreeList.
     * The block group will have zero blocks in it, so blocks must be added before
     * the group can be used. Nothing is initialized if fl == NULL.
     * @param fl The FreeList that the new BlockGroup will get nodes from.
     */
    BlockGroup(FreeList* fl);

    /**
     * Default constructor. Does not initialize anything.
     */
    BlockGroup();

    /**
     * Enables you to re-open a BlockGroup. Assumes that the BlockGroup is not
     * currently open. This enables you to re-open a file on subsequent runs of
     * testing programs, after an initial run when a BlockGroup was created but
     * not deallocated.
     * @param startBlock is the starting block number of the BlockGroup.
     * @param endBlock is the ending block number of the BlockGroup.
     * @param numberOfBlocks is the number of blocks in this BlockGroup.
     * @param motherFreeList is the BlockGroup's originating Free List.
     */
    BlockGroup(int startBlock, int endBlock, int numberOfBlocks, FreeList* motherFreeList);

    /**
     * Add another (empty) block to the end of this BlockGroup, from the Free List.
     * You need to make sure the Free List has blocks left in it before calling this method.
     * @return true iff Block could be added.
     */
    bool addBlock();

private:
    // These variables aren't needed, can use protected superclass variables
//    int startBlock;
//    int endBlock;
//    int numberOfBlocks;
    BlockLinkedList* blockLinkedList;
    FreeList* motherFreeList;

protected:
    friend class FreeList;

};

#endif	/* BLOCKGROUP_H */

