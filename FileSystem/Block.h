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
public:
    Block(int);
    Block(int,int);
    Block(int,Disk*);
    ~Block();
};



#endif	/* BLOCK_H */

