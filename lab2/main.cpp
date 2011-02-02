/* 
 * File:   main.cpp
 * Author: James Ekstrom
 * CS 5631 Lab 2
 * Created on February 1, 2011, 2:35 PM
 */

#include <cstdlib>

#include "Archive/Disk.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    //test
    Disk disk1("disk1",100,Disk::DEFAULT_BLOCK_SIZE);
    
    //this is a test
    printf("test");
    
    disk1.Test();
    
    return 0;
}

