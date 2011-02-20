/* 
 * File:   Entry.h
 * Author: james
 *
 * Created on February 19, 2011, 5:08 PM
 */

#ifndef ENTRY_H
#define	ENTRY_H
#include <string>

using namespace std;

class Entry {
public:

    Entry(int f,string s) {
        fcb = f;
        name = s;
    };

    int fcb;
    
    string name;

};
#endif	/* ENTRY_H */

