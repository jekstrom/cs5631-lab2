/* 
 * File:   BlockLinkedList.h
 * Author: james
 *
 * Created on February 2, 2011, 4:35 PM
 */

#ifndef BLOCKLINKEDLIST_H
#define	BLOCKLINKEDLIST_H

#include "Disk.h"
#include "Block.h"

class BlockLinkedList
{
public:
    
//    /**
//     * Default initialization for all data members. Does not modify the Disk.
//     */
//    BlockLinkedList();
    
    /**
     * Uses the disk and block size given to initialize those data members of the object. 
     * Other data members get default initialization. Does not modify the Disk.
     * @param disk to use in operations on the BlockLinkedList thus constructed. 
     * @param blockSize Total size of the blocks on this disk.
     */
    BlockLinkedList(Disk* disk, int blkSize);
		
    /**
     * Create a new BlockLinkedList using the disk, and block size of
     * another BlockLinkedList. Does not modify the Disk.
     * @param b Data member values are copied over from this other BlockLinkedList. 
     */
    BlockLinkedList(BlockLinkedList* b);

    ~BlockLinkedList()
    {
        delete currentBlockPtr;
    }
    
    /**
     * Add another block to the end of this BlockLinkedList. Makes changes on disk. 
     * Assumes that the ending block number for this BlockLinkedList is valid.
     * @param newBlock	is the new Block. This block should have already been 
     * removed from the BlockLinkedList upon which it was previously. 
     * Resets the data in the m_buffer of this block before writing to disk. 
     * @return true iff the new block was successfully added to the list.
     */
    bool addBlock(Block* newBlock);
    
    /**
     * Count the number of blocks in a BlockLinkedList by traversing the Blocks 
     * from the start Block to the end Block. This may be useful for debugging. 
     * The rewind state is advanced to the end of the list.
     * @return The number of blocks found by the traversal
     */
    int countBlocks();
    
    /**
     * Returns the amount of "usable" data within each block on disk.
     * @return the block size in bytes of the blocks associated with the BlockLinkedList. 
     * The returned value will be 4 bytes less than the block size of the Disk associated 
     * with this BlockLinkedList because the first four bytes of each block's m_buffer are 
     * reserved for the "next pointer". 
     */
    int inline getBlockSize()
    {
        return blockSize;
    }
    
    /**
     * Returns a reference to the current Block in the Rewind/GetNextBlock sequence.
     * The caller is responsible for deallocating the block once it is no longer needed.
     * @return A pointer to the current block
     */
    Block* getCurrentBlock();
    
    /**
     * Return the ending block number for the BlockLinkedList. 
     * @return the block number of the last block in the list.
     */
    int inline getEndBlockNumber()
    {
        return endBlockNum;
    }
    
    /**
     * Seeks the BlockLinkedList to the next block, unless GetCurrentBlock() returns null. 
     * Changes the value returned by GetCurrentBlock() to the next Block in the file, 
     * unless no more Blocks exist. In this case, GetCurrentBlock() will now return null. 
     * No changes are made to the disk.
     */
    void getNextBlock();
    
    /**
     * Return the number of blocks currently in this BlockLinkedList.
     * @return The current length of the list in blocks.
     */
    int inline getNumberOfBlocks()
    {
        return numBlocks;
    }
    
    /**
     * Return the starting block number for the BlockLinkedList. 
     * @return The block number of the first block in the list.
     */
    int inline getStartBlockNumber()
    {
        return startBlockNum;
    }

    /**
     * Retrieve a reference to the Disk that this list reads from and writes to.
     * @return a pointer to the Disk for this list.
     */
    Disk* getDisk();
    
    /**
     * Generates a one block BlockLinkedList. Modifies the disk. Assumes the disk 
     * and block size data members of this BlockLinkedList have been properly initialized.
     * @param blockNumber is the block number on disk of the first (and only)
     * block of this BlockLinkedList. Resets the data in the m_buffer
     * of this block before writing to disk. 
     * @return 
     */
    bool initialize(int blockNumber);

    /**
     * Outputs to the console the sequence of block numbers comprising this
     * BlockLinkedList. Block numbers are separated by "\n". Rewind state is
     * advanced to the end of the list by this call.
     */
    void output();

    /**
     * Replaces the m_buffer of the GetCurrentBlock() Block on Disk of this
     * BlockLinkedList with the m_buffer of the block given as a parameter. The
     * first four bytes of the m_buffer of the blk parameter are modified and
     * replaced with the Disk block's next pointer. The block number of the blk
     * parameter is also modified.
     * @param blk A pointer to the Block to replace the current block.
     * @return
     */
    bool replace(Block* blk);

    /**
     * After this call, GetCurrentBlock will return the first block in the
     * BlockLinkedList. After this call, a call to GetNextBlock will seek to the
     * second Block (if any) in the BlockLinkedList. No changes are made to disk.
     */
    void inline rewind()
    {
        currentBlockNum = startBlockNum;
        currentCalled = false;
    }

    /**
     * Unlinks the block that is the starting block of this BlockLinkedList.
     * Does not make changes on disk.
     * @return the Block representing the unlinked block. This Block isn't linked
     * into any BlockLinkedList. The next pointer of the Block will still reference
     * the Block to which it was previously linked. It is the responsibility of
     * the caller to deallocate the returned Block.
     */
    Block* unlinkBlock();

private:

    /**
     * A block with this as its pointer is the last block in the list.
     */
    const static int END_OF_LIST;

    /**
     * The number of blocks currently in the linked list.
     */
    int numBlocks;

    /**
     * The number of usable bytes in each block of the list.
     */
    int blockSize;

    /**
     * Block number of the first block in the linked list.
     */
    int startBlockNum;

    /**
     * Block number of the last block in the linked list.
     */
    int endBlockNum;

    /**
     * The block number of the current block.
     */
    int currentBlockNum;

    /**
     * A pointer to the current block in the list.
     */
    Block* currentBlockPtr;

    /**
     * Keeps track of whether or not getCurrentBlock has been called since the
     * last change of the current block pointer.
     */
    bool currentCalled;

    /**
     * A pointer to the Disk that the list will read from and write to.
     */
    Disk* diskPtr;
};

#endif	/* BLOCKLINKEDLIST_H */

