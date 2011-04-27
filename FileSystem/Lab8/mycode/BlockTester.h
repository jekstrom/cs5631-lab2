/* 
 * File:   BlockTester.h
 * Author: James
 *
 * Created on February 6, 2011, 4:39 PM
 */

#ifndef BLOCKTESTER_H
#define	BLOCKTESTER_H
#include "Block.h"

class BlockTester {
public:

    /**
     * Tests the block class
     */
    BlockTester() {
        const int LENGTH = Disk::DEFAULT_BLOCK_SIZE;
        char *deviceName = "disk1";
        unsigned char* input;
        int numberOfBlocks = 1000;
        int newBlockNumber = 0;
        int newBlockPointer = 0;
        int nextPointer = 0;
        int newLocation = 0;
        int cmd = 0;
        Block *block = NULL;
        Disk *disk1 = NULL;

        while(true) {
            printf("Test the Block class:\n");
            printf("0) Exit tester\n");
            printf("1) Create Block\n");
            printf("2) Create a Block with specific block number\n");
            printf("3) Set Block pointer\n");
            printf("4) Set Block next pointer\n");
            printf("5) Set Block buffer\n");
            printf("6) Write Block to disk\n");
            printf("7) Read block from disk\n");
            printf("8) Delete Block\n");
            scanf("%d", &cmd);

            switch(cmd) {
                case 0:
                    return;

                case 1:  //create default block
                    if (block == NULL) {
                    block = new Block(LENGTH);
                        printf("Created new Block of length %d bytes\n",LENGTH);
                        printf("Block number = %d\n",block->getBlockNumber());
                        printf("Block next pointer = %d\n",block->getNext());
                        printf("Block m_buffer = \n");
                        block->print();
                    }
                    else
                        printf("ERROR: Block already created, delete block first"
                                "then create a new one.\n");
                    
                    break;

                case 2:  //create block with specific block number
                    if (block == NULL) {
                        printf("Specify Block number: ");
                        scanf("%d", &newBlockNumber);
                        block = new Block(newBlockNumber, LENGTH);
                        printf("Created new Block of length %d bytes\n",LENGTH);
                        printf("Block number = %d\n",block->getBlockNumber());
                        printf("Block next pointer = %d\n",block->getNext());
                        printf("Block m_buffer = \n");
                        block->print();
                    }
                    else
                        printf("ERROR: Block already created, delete block first"
                                "then create a new one.\n");
                    break;

                case 3:  //set block pointer
                    if (block != NULL) {
                        printf("Specify new pointer value: ");
                        scanf("%d",&newBlockPointer);
                        printf("Specify m_buffer location: ");
                        scanf("%d",&newLocation);
                        block->setPointer(newBlockPointer, newLocation);
                        printf("Block next pointer at location %d = %d",newLocation,
                            block->getPointer(newLocation));
                        printf("Block m_buffer = \n");
                        block->print();
                    }
                    else
                        printf("ERROR: No block created, create block first!\n");
                    break;

                case 4:  //set block next pointer
                    if (block != NULL) {
                        printf("Specify next pointer value: ");
                        scanf("%d",&nextPointer);
                        block->setNext(nextPointer);
                        printf("Block next pointer at location 0 = %d",block->getNext());
                        printf("Block m_buffer = \n");
                        block->print();
                    }
                    else
                        printf("ERROR: No block created, create block first!\n");
                    break;

                case 5:  //set block buffer
                    if (block != NULL) {
                        input = new unsigned char[LENGTH];
                        printf("some m_buffer contents: ");
                        scanf("%s",input);
                        block->setBuffer(input);
                        printf("Block length = %d bytes\n",LENGTH);
                        printf("Block number = %d\n",block->getBlockNumber());
                        printf("Block next pointer at location 0 = %d",block->getNext());
                        printf("Block m_buffer = \n");
                        block->print();
                    }
                    else
                        printf("ERROR: No block created, create block first!\n");
                    break;

                case 6:  //write block to disk
                    if (block != NULL) {
                        printf("Creating new disk named %s with %d blocks\n",
                                deviceName,numberOfBlocks);
                        disk1 = new Disk(deviceName, numberOfBlocks, Disk::DEFAULT_BLOCK_SIZE);
                        printf("SUCCESS: disk1 is now open.");
                        printf("Attempting to write to disk...\n");
                        if(block->write(disk1))
                            printf("SUCCESS: block wrote to disk.\n");
                        else
                            printf("ERROR: block did not write to disk.\n");
                    }
                    else
                        printf("ERROR: No block created, create block first!\n");
                    break;

                case 7:  //read block from disk
                    if (disk1 != NULL) {
                        int blockNumber;
                        unsigned char* buffer;
                        printf("Which block number do you want to read?\n");
                        scanf("%d", &blockNumber);
                        printf("\n");
                        printf("Attempting to read block %d\n", blockNumber);

                        if (! disk1->ReadBlock(blockNumber,buffer)) {
                            printf("ERROR: Block read failed!\n");
                        }
                        else {
                            printf("SUCCESS: Block read!\n");
                            for (int i=0; i < Disk::DEFAULT_BLOCK_SIZE; i++) {
                                printf("%d,", buffer[i]);
                            }
                            printf("\n");
                        }
                    }
                    else
                        printf("ERROR: No disk.\n");
                    break;

                case 8: //delete block
                    block = NULL;
                    break;

            }
        }
    }
};

#endif	/* BLOCKTESTER_H */

