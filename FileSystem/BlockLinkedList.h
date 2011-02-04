/* 
 * File:   BlockLinkedList.h
 * Author: james
 *
 * Created on February 2, 2011, 4:35 PM
 */

#ifndef BLOCKLINKEDLIST_H
#define	BLOCKLINKEDLIST_H

class BlockLinkedList {
public:
    
    /**
     * Default initialization for all data members. Does not modify the Disk. 
     */
    BlockLinkedList();
    
    /**
     * Uses the disk and block size given to initialize those data members of the object. 
     * Other data members get default initialization. Does not modify the Disk.
     * @param disk to use in operations on the BlockLinkedList thus constructed. 
     * @param blockSize size of the blocks in this BlockLinkedList.
     */
    BlockLinkedList(Disk* disk, int blockSize);
		
    /**
     * Create a new BlockLinkedList using the disk, and block size of
     * another BlockLinkedList. Does not modify the Disk.
     * @param b Data member values are copied over from this other BlockLinkedList. 
     */
    BlockLinkedList(BlockLinkedList* b);
    
    /**
     * Add another block to the end of this BlockLinkedList. Makes changes on disk. 
     * Assumes that the ending block number for this BlockLinkedList is valid.
     * @param newBlock	is the new Block. This block should have already been 
     * removed from the BlockLinkedList upon which it was previously. 
     * Resets the data in the m_buffer of this block before writing to disk. 
     * @return 
     */
    bool AddBlock(Block* newBlock);
    
    /**
     * Count the number of blocks in a BlockLinkedList by traversing the Blocks 
     * from the start Block to the end Block. This may be useful for debugging. 
     * Changes the Rewind state of the BlockLinkedList. 
     * @return 
     */
    int CountBlocks();
    
    /**
     * Returns the amount of "usable" data within each block on disk.
     * @return the block size in bytes of the blocks associated with the BlockLinkedList. 
     * The returned value will be 4 bytes less than the block size of the Disk associated 
     * with this BlockLinkedList because the first four bytes of each block's m_buffer are 
     * reserved for the "next pointer". 
     */
    int GetBlockSize();
    
    /**
     * Returns a reference to the current Block in the Rewind/GetNextBlock sequence. 
     * @return 
     */
    Block* GetCurrentBlock();
    
    /**
     * Return the ending block number for the BlockLinkedList. 
     * @return 
     */
    int GetEndBlockNumber();
    
    /**
     * Seeks the BlockLinkedList to the next block, unless GetCurrentBlock() returns null. 
     * Changes the value returned by GetCurrentBlock() to the next Block in the file, 
     * unless no more Blocks exist. In this case, GetCurrentBlock() will now return null. 
     * No changes are made to disk. 
     */
    void GetNextBlock();
    
    /**
     * Return the number of blocks currently in this BlockLinkedList.
     * @return 
     */
    int GetNumberOfBlocks();
    
    /**
     * Return the starting block number for the BlockLinkedList. 
     * @return 
     */
    int GetStartBlockNumber 	( 		 );
    
    /**
     * Generates a one block BlockLinkedList. Modifies the disk. Assumes the disk 
     * and block size data members of this BlockLinkedList have been properly initialized.
     * @param blockNumber blockNumber 	is the block number on disk of the first 
     * (and only) block of this BlockLinkedList. Resets the data in the m_buffer 
     * of this block before writing to disk. 
     * @return 
     */
    bool Initialize 	( 	int  	blockNumber 	 );
};



#endif	/* BLOCKLINKEDLIST_H */

