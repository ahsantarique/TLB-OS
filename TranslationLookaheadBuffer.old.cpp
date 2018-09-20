#include <map>
#include <iostream>
#include "configuration.h"
#include "LookupResult.cpp"
#include <cstdio>

using namespace std;
#ifndef TranslationLookaheadBuffer_Class
#define TranslationLookaheadBuffer_Class

class TLBEntry
{
public:
    unsigned char virtualPageNo;
    unsigned char physicalFrameIndex;
    bool isEmpty;

    TLBEntry(){
        isEmpty=true;
    }
};


class TranslationLookaheadBuffer
{
    TLBEntry * entries;
    int cacheSize;
    int usedCounter;

public:

    /*
    * Add necessary class variables here
    */

    TranslationLookaheadBuffer(int size)
    {
        //write your code here
        entries = new TLBEntry[size];
        cacheSize = size;
        usedCounter = 0;
    }

    /*
    * Append a new TLB Entry into the end of the LRU Linked List
    * Will only be used in the LRU Implementation
    */
    void append(TLBEntry *t)
    {
        //write your code here
    }
    /*
    * Detach a TLB Entry from the LRU Linked List
    * Will only be used in the LRU Implementation
    */
    void detach(TLBEntry *t)
    {
        //write your code here
    }

    /*
    ******************************************************
    * my code starts here
    *******************************************************
    */

    LookupResult lookup(unsigned char pageNumber){
        //write your code here
        LookupResult ret;
        ret.isFound = false;

        for(int i=0; i <cacheSize; i++){
            if(!entries[i].isEmpty && pageNumber == entries[i].virtualPageNo){
                ret.isFound = true;
                ret.frameIndex=entries[i].physicalFrameIndex;
                break;
            }
        }
        return ret;
    }



    void insert(unsigned char pageNumber, unsigned char frameIndex)
    {
        //write your code here
        bool emptyFound = false;
        for(int i=0; i < cacheSize; i++){
            if(entries[i].isEmpty){
                emptyFound = true;

                entries[i].virtualPageNo=pageNumber;
                entries[i].physicalFrameIndex=frameIndex;
                entries[i].isEmpty=false;
                return;
            }
        }
        if(!emptyFound){
            entries[0].virtualPageNo=pageNumber;
            entries[0].physicalFrameIndex=frameIndex;
            //entries[0]->isEmpty=false;
        }
        print();
    }

    void print()
    {
        //write your code here
		printf("PRINTING TLB\n");
        for(int i = 0; i< cacheSize; i++){
			if(!entries[i].isEmpty){
				printf("< Virtual Page no. %02x and Physical Frame no. %02x > \n", entries[i].virtualPageNo, entries[i].physicalFrameIndex);
			}
        }
		printf("FINISHED PRINTING TLB\n");
    }

    void invalidate(unsigned char pageNumber)
    {
        //write your code here
        for(int i = 0; i < cacheSize; i++){
            if(entries[i].virtualPageNo==pageNumber){
                entries[i].isEmpty = true;
                break;
            }
        }
    }
};
#endif
