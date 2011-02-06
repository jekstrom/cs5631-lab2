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
        const int LENGTH = 512;
        char *deviceName = "disk1";
        int numberOfBlocks = 1000;
        int cmd = 0;
        Block *block = NULL;
        Disk *disk1 = NULL;

        while(true) {
            printf("Test the Block class:");
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
                        printf("Block number = %d\n",block.getBlockNumber());
                        printf("Block next pointer = %d",block.getNext());
                        printf("Block m_buffer = \n");
                        block.print();
                    }
                    else
                        printf("ERROR: Block already created, delete block first"
                                "then create a new one.\n");
                    
                    break;

                case 2:  //create block with specific block number
                    int newBlockNumber = 0;
                    if (block == NULL) {
                        printf("Specify Block number: ");
                        scanf("%d\n", &newBlockNumber);
                        block = new Block(LENGTH, newBlockNumber);
                        printf("Created new Block of length %d bytes\n",LENGTH);
                        printf("Block number = %d\n",block.getBlockNumber());
                        printf("Block next pointer = %d",block.getNext());
                        printf("Block m_buffer = \n");
                        block.print();
                    }
                    else
                        printf("ERROR: Block already created, delete block first"
                                "then create a new one.\n");
                    break;

                case 3:  //set block pointer
                    int newBlockPointer = 0;
                    int newLocation = 0;
                    if (block != NULL) {
                        printf("Specify new pointer value: ");
                        scanf("%d\n",&newBlockPointer);
                        printf("Specify m_buffer location: ");
                        scanf("%d\n",&newLocation);
                        block.setPointer(newBlockPointer, newLocation);
                        printf("Block next pointer at location %d = %d",newLocation,
                            block.getPointer(newLocation));
                        printf("Block m_buffer = \n");
                        block.print();
                    }
                    else
                        printf("ERROR: No block created, create block first!\n");
                    break;

                case 4:  //set block next pointer
                    int nextPointer = 0;
                    if (block != NULL) {
                        printf("Specify next pointer value: ");
                        scanf("%d\n",&nextPointer);
                        block.setNext(nextPointer);
                        printf("Block next pointer at location 0 = %d",block.getNext());
                        printf("Block m_buffer = \n");
                        block.print();
                    }
                    else
                        printf("ERROR: No block created, create block first!\n");
                    break;

                case 5:  //set block buffer
                    unsigned char* input[LENGTH] = { NULL };
                    if (block != NULL) {
                        printf("some m_buffer contents: ");
                        scanf("%s\n",input);
                        block.setBuffer(input);
                        printf("Block length = %d bytes\n",LENGTH);
                        printf("Block number = %d\n",block.getBlockNumber());
                        printf("Block next pointer at location 0 = %d",block.getNext());
                        printf("Block m_buffer = \n");
                        block.print();
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
                        if(block.write(disk1))
                            printf("SUCCESS: block wrote to disk.\n");
                        else
                            printf("ERROR: block did not write to disk.\n");
                    }
                    else
                        printf("ERROR: No block created, create block first!\n");
                    break;

                case 7:  //read block from disk
                    if (disk1 != NULL) {
                        
                    }
                    else
                        printf("ERROR: No disk.\n");
                    break;

            }
        }
    }
};

#endif	/* BLOCKTESTER_H */

