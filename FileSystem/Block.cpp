/*
 * File:   Block.cpp
 * Author: James Ekstrom
 *
 * Created on February 4, 2011, 4:05 PM
 */

#include "Block.h"

Block::Block(int blockNumber, int length) {
    this->blockNumber = blockNumber;
    this->length = length;
    m_buffer = new unsigned char[length];
    for (int i = 0; i < this->length; i++)
        m_buffer[i] = 0;
}

Block::Block(int length) {
    this->length = length;
    blockNumber = 0;
    m_buffer = new unsigned char[length];
    for (int i = 0; i < this->length; i++)
        m_buffer[i] = 0;
}

Block::Block(int blockNumber, Disk* disk) throw(CannotReadException) {
    //unsigned char block[Disk::DEFAULT_BLOCK_SIZE];
    length = disk->blockSize() - sizeof(int);
    m_buffer = new unsigned char[disk->blockSize()];
    this->blockNumber = blockNumber;
    if(!disk->ReadBlock(blockNumber, m_buffer))
         throw new CannotReadException();
}

Block::~Block() {
    delete[] m_buffer;
}

void Block::clearBuffer() {
    for(int i = 0; i < this->length; i++)
        m_buffer[i] = 0;
}

void Block::setBuffer(unsigned char* buffer) {
    delete[] m_buffer;
    m_buffer = buffer;
}

unsigned char* Block::getBuffer() {
    return m_buffer;
}

void Block::setBlockNumber(int newBlockNumber) {
    this->blockNumber = newBlockNumber;
}

int Block::getBlockNumber() {
    return this->blockNumber;
}

void Block::setPointer(int pointer, int location) {
    char* p = (char*) &pointer;
    int bufferIndex = location*sizeof(int);
    for(int i = 0; i < sizeof(int); i++) {
        m_buffer[bufferIndex] = p[i];
        bufferIndex++;
    }
}

int Block::getPointer(int location) {
    char integer[sizeof(int)] = {0};
    int bufferIndex = location * 4;
    for (int i = 0; i < sizeof(int); i++)
    {
        integer[i] = m_buffer[bufferIndex];
        bufferIndex++;
    }
    int pointer = *((int*) integer);
    return pointer;
}

void Block::setNext(int pointer) {
    setPointer(pointer,0);
}

int Block::getNext() {
    return this->getPointer(0);
}

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

bool Block::write(Disk* disk) {
    return disk->WriteBlock(blockNumber, m_buffer);
}