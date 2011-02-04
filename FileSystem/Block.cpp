/*
 * File:   Block.cpp
 * Author: James Ekstrom
 *
 * Created on February 4, 2011, 4:05 PM
 */

#include "Block.h"


/**A constructor for the block class
 * Creates a RAM representation of a block of data.
 * Doesn't do any error checking.
 * Initializes the buffer of data (data member, m_buffer) to zeros.
 * Does not alter the disk.
 * @param blockNumber the number of the block
 * @param length the number of bytes to allocate for the m_buffer
 */
Block::Block(int blockNumber, int length) {
    this->blockNumber = blockNumber;
    this->length = length;

    for (int i = 0; i < this->length; i++)
        m_buffer[i] = 0;
}

/**A constructor for the block class
 * Creates a Block. Initializes the buffer of data to zeros.
 * The block number data member is set to 0. Does not alter the disk.
 * @param length the number of bytes to allocate for the m_buffer
 */
Block::Block(int length) {
    this->length = length;

    for (int i = 0; i < this->length; i++)
        m_buffer[i] = 0;
}

/**Attempts to read a Block from a Disk.
 * If the block as written to Disk had a next or second pointer value,
 * that next or second pointer value will be available after use of this
 * constructor by calling GetPointer().
 * @param blockNumber The block number of the Block to read.
 * This can range between 0 and GetNumberOfBlocks()-1 (inclusive) for this Disk
 * object.
 * @param disk the Disk to read from
 */
Block::Block(int blockNumber, Disk* disk) {
    unsigned char* block;
    this->blockNumber = blockNumber;
    if(disk.ReadBlock(blockNumber,block)) 
        m_buffer = block;
    else
        throw new CannotReadException();
}

/**The Block deconstructor
 */
Block::~Block() {}

/**Set all bytes of this buffer to zero
 * Does not alter the Disk
 */
void Block::clearBuffer() {
    
}

/**Changes the reference of the m_buffer. No copy is made!
 * Memory for the original m_buffer is first deallocated by this method
 * (using "delete").
 * @param buffer is used by reference
 */
void Block::setBuffer(unsigned char* buffer) {
    
}

/**Obtain a reference to the m_buffer data member for this Block. 
 * @return a reference to the byte buffer for this Block
 */
unsigned char* Block::getBuffer() {

}

/**Changes the block number for this Block. Doesn't write the change to disk.
 * The block number is not represented by the m_buffer, but rather it is
 * represented as a data member of the Block. The block number is the location
 * where you just read the block from on Disk, or the location where you intend
 *  to write the block to disk.
 * @param newBlockNumber the new block number
 */
void Block::setBlockNumber(int newBlockNumber) {

}

/**Returns the block number for this Block, returns value of a data member.
 * Does not access Disk.
 * @return the block number of the Block
 */
int Block::getBlockNumber() {

}

/**Changes one of the pointers in the m_buffer. If location=0,
 * the first 4 bytes of the m_buffer are changed to the binary value in the
 * pointer. ("first pointer") If location=1, the second 4 bytes are changed,
 * etc. ("second pointer") Etc.
 * @param pointer the next value
 * @param location gives the location (in terms of 4 byte ints) in the
 * m_buffer to change
 */
void Block::setNext(int pointer, int location) {

}

/**Retrieves one of the pointers in the m_buffer
 * @param location gives the relative 4 byte location to access. See SetPointer.
 * @return the next pointer
 */
int Block::getNext(int location) {

}

/**Print out the contents of the Block. Prints the m_buffer bytes in hex and
 * decimal representation. Prints out the values of the other data members.
 * For debugging.
 */
void Block::print() {

}

/**Writes the data from the Block to its block number on the Disk. The Block 
 * must have the correct information. E.g., this can be the case because a 
 * previous Block constructor reading from a Disk returned the Block.
 * @param disk The Disk to write this Block to. 
 * @return true iff the call succeeded in writing the Block to the Disk.
 */
bool Block::write(Disk* disk) {

}