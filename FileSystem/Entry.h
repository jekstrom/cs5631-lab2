/* 
 * File:   Entry.h
 * Author: james
 *
 * Created on February 19, 2011, 5:08 PM
 */

#ifndef ENTRY_H
#define	ENTRY_H
#include <string>

class Entry {
public:

    Entry(int f,std::string s) {
        fcb = f;
        name = s;
    };

    int fcb;
    
    std::string name;

};
#endif	/* ENTRY_H */

