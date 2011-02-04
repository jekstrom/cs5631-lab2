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
    char* m_buffer;
    int next;
public:
    Block(int);
    Block(int,int);
    Block(int,Disk*);
    ~Block();

    void clearBuffer();
    void setBuffer(unsigned char*);
    unsigned char* getBuffer();

    void setBlockNumber(int);
    int getBlockNumber();

    void setNext(int,int);
    int getNext(int);
    
    void print();
    bool write(Disk*);

};



#endif	/* BLOCK_H */

