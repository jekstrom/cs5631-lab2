/* 
 * File:   BlockLinkedList.h
 * Author: James Ekstrom, Kaleb Jungclaus, Matt Fischer
 *
 * Created on February 2, 2011, 4:35 PM
 */

#ifndef BLOCKLINKEDLIST_H
#define	BLOCKLINKEDLIST_H

#include "Disk.h"
#include "Block.h"
#include "CannotReadException.h"

class BlockLinkedList
{
public:
    
    /**
     * Default initialization for all data members. Does not modify the Disk.
     */
    BlockLinkedList();

    /**
     * A block with this as its pointer is the last block in the list.
     */
    const static int END_OF_LIST = 0;

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
    }
    
    /**
     * Add another block to the end of this BlockLinkedList. Makes changes on disk. 
     * Assumes that the ending block number for this BlockLinkedList is valid.
     * @param newBlock	is the new Block. This block should have already been 
     * removed from the BlockLinkedList upon which it was previously. 
     * Resets the data in the m_buffer of this block before writing to disk. 
     * @return true iff the new block was successfully added to the list.
     */
    bool addBlock(Block* newBlock) throw(CannotReadException);
    
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
    int getBlockSize()
    {
        return blockSize;
    }
    
    /**
     * Returns a reference to the current Block in the Rewind/GetNextBlock sequence.
     * The caller is responsible for deallocating the block once it is no longer needed.
     * Each call will return a pointer to a different Block object.
     * @return A pointer to the current block
     */
    Block* getCurrentBlock();
    
    /**
     * Return the ending block number for the BlockLinkedList. 
     * @return the block number of the last block in the list.
     */
    int getEndBlockNumber()
    {
        return endBlockNum;
    }
    
    /**
     * Seeks the BlockLinkedList to the next block, unless GetCurrentBlock() returns null. 
     * Changes the value returned by GetCurrentBlock() to the next Block in the file, 
     * unless no more Blocks exist. In this case, GetCurrentBlock() will now return null. 
     * No changes are made to the disk.
     * @return true if not end of list
     */
    bool getNextBlock();
    
    /**
     * Return the number of blocks currently in this BlockLinkedList.
     * @return The current length of the list in blocks.
     */
    int getNumberOfBlocks()
    {
        return numBlocks;
    }
    
    /**
     * Return the starting block number for the BlockLinkedList. 
     * @return The block number of the first block in the list.
     */
    int getStartBlockNumber()
    {
        return startBlockNum;
    }

    /**
     * Retrieve a reference to the Disk that this list reads from and writes to.
     * @return a pointer to the Disk for this list.
     */
    Disk* getDisk()
    {
        return diskPtr;
    }
    
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
     * The idea in this method is to replace the data of the m_seek Block
     * (i.e., the GetCurrentBlock() Block) but to leave the link structure of the
     * BlockLinkedList alone and unchanged. Replaces the last blk.GetBlockSize()-4
     * bytes of the GetCurrentBlock() Block on Disk of this BlockLinkedList with
     * the last blk.GetBlockSize()-4 bytes of the m_buffer of the block given as
     * a parameter. The first four bytes of the m_seek Block (i.e., the GetCurrentBlock() Block)
     * are retained. This modifies the first four bytes of the m_buffer of the blk
     * parameter, and the block number of the blk parameter is also modified.
     * Modifies the Disk.
     * @param blk A pointer to the Block to replace the current block.
     * @return true iff the replacement is successful
     */
    bool replace(Block* blk);

    /**
     * After this call, GetCurrentBlock will return the first block in the
     * BlockLinkedList. After this call, a call to GetNextBlock will seek to the
     * second Block (if any) in the BlockLinkedList. No changes are made to disk.
     */
    void rewind();

    /**
     * Unlinks the block that is the starting block of this BlockLinkedList.
     * Does not make changes on disk If the starting block could not be read,
     * NULL will be returned. Will also return NULL if the current block
     * could not be read from.
     * @return the Block representing the unlinked block. This Block isn't linked
     * into any BlockLinkedList. The next pointer of the Block will still reference
     * the Block to which it was previously linked. It is the responsibility of
     * the caller to deallocate the returned Block.
     */
    Block* unlinkBlock();

    /**
     * Tests the methods of the class.
     */
    void test();

protected:

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
     * A pointer to the Disk that the list will read from and write to.
     */
    Disk* diskPtr;

private:

    // Test methods. Each assumes that the previous test methods have been called.
    void test1();
    void test2();
    void test3();
    void test4();
    void test5();
};

#endif	/* BLOCKLINKEDLIST_H */

