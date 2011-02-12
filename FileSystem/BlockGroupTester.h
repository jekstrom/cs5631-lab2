/* 
 * File:   BlockGroupTester.h
 * Author: james
 *
 * Created on February 12, 2011, 11:04 AM
 */

#ifndef BLOCKGROUPTESTER_H
#define	BLOCKGROUPTESTER_H
#include "BlockGroup.h"

class BlockGroupTester {

public:

    /**
     * Tests the BlockGroup class
     */
    BlockGroupTester() {
        int cmd = 0;
        BlockGroup *bg = NULL;
        BlockLinkedList *bll = NULL;
        FreeList *fl = NULL;

        while(true) {
            printf("Test the BlockGroup class\n");
            printf("0) Exit tester\n");
            printf("1) Create a new BlockGroup initialized with a BlockLinkedList\n");
            printf("2) Create a new BlockGroup initialized with a FreeList\n");
            printf("3) Open a BlockGroup\n");
            printf("4) Add empty Block to BlockGroup");
            scanf("%d", &cmd);

            switch(cmd) {
                case 0: //exit
                    return;
                    
                case 1: //Create a new BlockGroup initialized with a BlockLinkedList
                    printf("Attempting to create a new BlockGroup...\n");
                    printf("Initializing BlockLinkedList...\n");
                    bll->initialize(0);
                    printf("BlockLinkedList info:\n")
                    printf("First Block Number = %d\n",bll->getStartBlockNumber());
                    printf("Last Block Number = %d\n",bll->getEndBlockNumber());
                    printf("Number of Blocks = %d\n",bll->getNumberOfBlocks());

                    printf("Setting up MasterBlock\n");
                    bll->getCurrentBlock()->setPointer(0,0); //first block number
                    bll->getCurrentBlock()->setPointer(0,1); //last block number
                    bll->getCurrentBlock()->setPointer(1,2); //number of blocks

                    printf("Creating BlockGroup with 1 Block BlockLinkedList\n");
                    bg = new BlockGroup(bll);
                    bg->output();

                    break;

                case 2: //Create a new BlockGroup initialized with a FreeList
                    if (bg != NULL) {
                        printf("Attempting to create a new BlockGroup...\n");
                        printf("Initializing FreeList...\n");
                        fl = new FreeList("testfile",1);
                        
                        BlockGroup freebg = fl->createNew();
                        printf("FreeList: \n");
                        freebg->output();

                        printf("Creating new BlockGroup\n");
                        bg = new BlockGroup(fl);
                        bg->output();
                    }
                    else {
                        printf("BlockGroup already initialized!\n");
                        break;
                    }
                    break;

                case 3:  //Open a BlockGroup
                    printf("Attempting to open previously created BlockGroup...\n");
                    if (bll != NULL) {
                        printf("Using Previously created BlockLinkedList data\n");
                        printf("BlockLinkedList info:\n")
                        printf("First Block Number = %d\n",bll->getStartBlockNumber());
                        printf("Last Block Number = %d\n",bll->getEndBlockNumber());
                        printf("Number of Blocks = %d\n",bll->getNumberOfBlocks());
                        bg = new BlockGroup(bll->getStartBlockNumber(),bll->getEndBlockNumber(),
                            bll->getNumberOfBlocks(),fl);
                        bg->output();
                    }
                    else if (fl != NULL) {
                        printf("Using Previously created FreeList data\n");
                        printf("FreeList info:\n")
                        printf("First Block Number = %d\n",fl->getStartBlockNumber());
                        printf("Last Block Number = %d\n",fl->getEndBlockNumber());
                        printf("Number of Blocks = %d\n",fl->getNumberOfBlocks());
                        bg = new BlockGroup(fl->getStartBlockNumber(),fl->getEndBlockNumber(),
                            fl->getNumberOfBlocks());
                        bg->output();
                    }
                    else {
                        printf("No FreeList or BlockLinkedList was created!\n");
                        break;
                    }
                    break;

                case 4: //Add empty Block to BlockGroup
                    if (bg != NULL) {
                        printf("Attempting to add empty Block to BlockGroup\n");
                        bg->addBlock();
                        bg->output();
                    }
                    else {
                        printf("BlockGroup doesn't exist!\n");
                        break;
                    }

                    break;
            } //end switch
        }
    }
};


#endif	/* BLOCKGROUPTESTER_H */

