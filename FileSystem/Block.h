/* 
 * File:   Block.h
 * Author: James Ekstrom
 *
 * Created on February 2, 2011, 4:35 PM
 */

#ifndef BLOCK_H
#define	BLOCK_H

#include "Disk.h"

class Block {
private:
    int blockNumber;
    int length;
    unsigned char* m_buffer;
    int next;
public:

    /**
     * Creates a Block. Initializes the buffer of data to zeros. The block number
     * data member is set to 0. Does not alter the disk.
     * @param length is the number of bytes to allocate for the m_buffer.
     */
    Block(int length);

    /**
     * Creates a RAM representation of a block of data. Doesn't do any error
     * checking. Initializes the buffer of data (data member, m_buffer) to zeros.
     * Does not alter the disk.
     * @param blockNumber should range betwen 1 and (1 less than the number of
     * blocks on the Disk). This is used to set a data member for the Block.
     * @param length should be the same as the block size on the Disk. Allocates
     * a new byte array m_buffer.
     */
    Block(int blockNumber,int length);

    /**
     * Attempts to read a Block from a Disk. If the block as written to Disk had
     * a next or second pointer value, that next or second pointer value will be
     * available after use of this constructor by calling GetPointer().
     * @param blockNumber The block number of the Block to read. This can
     * range between 0 and GetNumberOfBlocks()-1 (inclusive) for this Disk object.
     * @param disk is the Disk to read from.
     * @throw CannotReadException if the disk could not be read from.
     */
    Block(int blockNumber, Disk* disk) throw(CannotReadException);

    /**
     * Deallocates the m_buffer for the Block.
     */
    ~Block();

    /**
     * Set all bytes of the buffer of this Block to 0. Does not alter the disk.
     */
    void clearBuffer();

    /**
     * Changes the reference of the m_buffer. No copy is made! Memory for the
     * original m_buffer is first deallocated by this method (using "delete").
     * @param buffer is used by reference-- a copy is not made.
     */
    void setBuffer(unsigned char* buffer);

    /**
     * Obtain a reference to the m_buffer data member for this Block.
     * @return a reference to (not a copy of) the byte buffer for this Block.
     */
    unsigned char* getBuffer();

    /**
     * Changes the block number for this Block. Doesn't write the change to disk.
     * The block number is not represented by the m_buffer, but rather it is
     * represented as a data member of the Block. The block number is the location
     * where you just read the block from on Disk, or the location where you
     * intend to write the block to disk.
     * @param number The new number for this Block.
     */
    void setBlockNumber(int number);

    /**
     * Returns the block number for this Block, returns value of a data member. Does not access Disk.
     * @return
     */
    int getBlockNumber();

    /**
     * Changes one of the pointers in the m_buffer. If location=0, the first 4 bytes
     * of the m_buffer are changed to the binary value in the pointer. ("first pointer")
     * If location=1, the second 4 bytes are changed, etc. ("second pointer") Etc.
     * @param pointer gives the pointer value
     * @param location gives the location (in terms of 4 byte ints) in the m_buffer to change
     */
    void setPointer(int pointer, int location);

    /**
     * Retrieves one of the pointers in the m_buffer.
     * @param location gives the relative 4 byte location to access. See SetPointer.
     * @return the indicated pointer
     */
    int getPointer(int location);

    /**
     * Changes the pointer in the m_buffer to point to the given value
     * @param pointer the next block
     */
    void setNext(int pointer);

    /**
     * Retrieves the pointer to the next Block in the m_buffer
     * @return the pointer to the next block
     */
    int getNext();

    /**
     * Print out the contents of the Block. Prints the m_buffer bytes in hex and
     * decimal representation. Prints out the values of the other data members.
     */
    void print();

    /**
     * Writes the data from the Block to its block number on the Disk. The Block
     * must have the correct information. E.g., this can be the case because a
     * previous Block constructor reading from a Disk returned the Block.
     * @param disk The Disk to write this Block to.
     * @return true iff the call succeeded in writing the Block to the Disk.
     */
    bool write(Disk* disk);

};



#endif	/* BLOCK_H */

