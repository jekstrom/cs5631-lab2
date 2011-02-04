/*
 * File:   Block.cpp
 * Author: James Ekstrom
 *
 * Created on February 4, 2011, 4:05 PM
 */
#include "cs5631-lab2/trunk/lab2/Block.h"

/* A constructor for the block class
 * Creates a RAM representation of a block of data.
 * Doesn't do any error checking.
 * Initializes the buffer of data (data member, m_buffer) to zeros.
 * Does not alter the disk.
 * @param int blockNumber
 * @param int length
 */
Block::Block(int blockNumber, int length) {
    this->blockNumber = blockNumber;
    this->length = length;

    for (int i = 0; i < this->length; i++)
        m_buffer[i] = 0;
}

/* A constructor for the block class
 * Creates a Block. Initializes the buffer of data to zeros.
 * The block number data member is set to 0. Does not alter the disk.
 */
Block::Block(int length) {
    this->length = length;

    for (int i = 0; i < this->length; i++)
        m_buffer[i] = 0;
}

/* */

/* The Block deconstructor
 */
Block::~Block() {}


