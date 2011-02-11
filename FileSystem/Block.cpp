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
    m_buffer = new unsigned char[length];
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
    m_buffer = new unsigned char[length];
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
Block::Block(int blockNumber, Disk* disk) throw(CannotReadException) {
    unsigned char block[Disk::DEFAULT_BLOCK_SIZE];
    this->blockNumber = blockNumber;
    if(disk->ReadBlock(blockNumber,block))
        m_buffer = block;
    else
        throw new CannotReadException();
}

/**The Block deconstructor
 */
Block::~Block() {
    delete[] m_buffer;
}

/**Set all bytes of this buffer to zero
 * Does not alter the Disk
 */
void Block::clearBuffer() {
    for(int i = 0; i < this->length; i++)
        m_buffer[i] = 0;
}

/**Changes the reference of the m_buffer. No copy is made!
 * Memory for the original m_buffer is first deallocated by this method
 * (using "delete").
 * @param buffer is used by reference
 */
void Block::setBuffer(unsigned char* buffer) {
    delete[] m_buffer;
    m_buffer = buffer;
}

/**Obtain a reference to the m_buffer data member for this Block. 
 * @return a reference to the byte buffer for this Block
 */
unsigned char* Block::getBuffer() {
    return m_buffer;
}

/**Changes the block number for this Block. Doesn't write the change to disk.
 * The block number is not represented by the m_buffer, but rather it is
 * represented as a data member of the Block. The block number is the location
 * where you just read the block from on Disk, or the location where you intend
 *  to write the block to disk.
 * @param newBlockNumber the new block number
 */
void Block::setBlockNumber(int newBlockNumber) {
    this->blockNumber = newBlockNumber;
}

/**Returns the block number for this Block, returns value of a data member.
 * Does not access Disk.
 * @return the block number of the Block
 */
int Block::getBlockNumber() {
    return this->blockNumber;
}

/**Changes one of the pointers in the m_buffer. If location=0,
 * the first 4 bytes of the m_buffer are changed to the binary value in the
 * pointer. ("first pointer") If location=1, the second 4 bytes are changed,
 * etc. ("second pointer") Etc.
 * @param pointer the next value
 * @param location gives the location (in terms of 4 byte ints) in the
 * m_buffer to change
 */
void Block::setPointer(int pointer, int location) {
    int *p;
    p = &pointer;
    char *p2 = (char*)p;
    int counter = 0;
    for(int i = location*sizeof(int); i < sizeof(int); i++) {
        m_buffer[i] = p2[counter];
        counter++;
    }
}

 /**
  * Retrieves one of the pointers in the m_buffer.
  * @param location gives the relative 4 byte location to access. See SetPointer.
  * @return the indicated pointer
  */
int Block::getPointer(int location) {
    char integer[sizeof(int)] = {0};
    int bufferIndex = location * 4;
    for (int i = 0; i < sizeof(int); i++)
    {
        integer[i] = m_buffer[bufferIndex];
        bufferIndex++;
    }
    int pointer = (int) integer;
    return pointer;
}

 /**
  * Changes the first 4 bytes in the m_buffer to point to the given value
  * @param pointer the next block
  */
void Block::setNext(int pointer) {
    setPointer(pointer,0);
}

 /**
  * Retrieves the pointer to the next Block in the m_buffer
  * @return the pointer to the next block
  */
int Block::getNext() {
    return this->getPointer(0);
}

/**Print out the contents of the Block. Prints the m_buffer bytes in hex and
 * decimal representation. Prints out the values of the other data members.
 * For debugging.
 */
void Block::print() {
    printf("\n--------------------------------\n");
    printf("m_buffer contents in hex:\n");
    for(int i = 0; i < this->length; i++) {
        printf("%x ",m_buffer[i]);
        if ((i+1 % 32) == 0) //every 32 digits, newline
            printf("\n");
    }
    printf("\n--------------------------------\n");
    printf("m_buffer contents in decimal:\n");
    for(int i = 0; i < this->length; i++) {
        printf("%d ",m_buffer[i]);
         if ((i+1 % 32) == 0)
            printf("\n");
    }
    printf("\n--------------------------------\n");
}

/**Writes the data from the Block to its block number on the Disk. The Block 
 * must have the correct information. E.g., this can be the case because a 
 * previous Block constructor reading from a Disk returned the Block.
 * @param disk The Disk to write this Block to. 
 * @return true iff the call succeeded in writing the Block to the Disk.
 */
bool Block::write(Disk* disk) {
    return disk->WriteBlock(blockNumber, m_buffer);
}