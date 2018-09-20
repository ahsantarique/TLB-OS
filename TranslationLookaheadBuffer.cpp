#include <map>
#include <iostream>
#include <cstdio>
#include <list>
#include "configuration.h"
#include "LookupResult.cpp"

using namespace std;
#ifndef TranslationLookaheadBuffer_Class
#define TranslationLookaheadBuffer_Class


unsigned rollingHash(unsigned char key){
    const int PRIME_BASE = 31;
    const int MOD = 255;
    unsigned ret = 0;
    for (int i = 0; i < 8; i++){
        ret *= PRIME_BASE; //shift over by one
        ret += ((key>>i)&1); //add the current char
        ret %= (MOD);
    }
    return ret;
}


class TLBEntry
{
public:
    unsigned char virtualPageNo;
    unsigned char physicalFrameIndex;
    TLBEntry *previous;
    TLBEntry *next;
    TLBEntry(){
        previous = NULL;
        next = NULL;
        virtualPageNo = '-';
        physicalFrameIndex= '-';
    }
};

class HashTableEntry{
public:
    list< pair <unsigned char, TLBEntry * > > hashedPointerList;
    HashTableEntry(){
        
    }      
};

class HashTable{
public:

    HashTableEntry *table;

    HashTable(int size){
        table = new HashTableEntry[size];
    }


    void insert(unsigned char pageNo, TLBEntry * t,int cacheSize){
        int hashValue=rollingHash(pageNo);
        hashValue %= cacheSize;
        // cout << "alright" << endl;
        HashTableEntry * pEntry = table+hashValue;
        // cout << "alright pEntry" << endl;

		pair <unsigned char, TLBEntry * > pairtoPush;
		pairtoPush.first = pageNo;
		pairtoPush.second= t;
		// cout << "ok here" << endl;
        (pEntry->hashedPointerList).push_front(pairtoPush);
        // for(list< pair <unsigned char, TLBEntry * > > :: iterator it= pEntry->hashedPointerList.begin(); it!=pEntry->hashedPointerList.end(); it++){

        // } 
        // cout << "alright at the end" << endl;       
    }
};



class TranslationLookaheadBuffer
{
public:
    /*
    * Add necessary class variables here
    */
    HashTable *hTable;
    TLBEntry *head;
    TLBEntry *tail;
    int cacheSize;

    TranslationLookaheadBuffer(int size)
    {
        //write your code here
        cacheSize = size;
        hTable = new HashTable(size);
        
        //first entry
        head = new TLBEntry;
        head->previous = NULL;
        head->next = NULL;
        tail= head;

        //iterate
        TLBEntry * cur = head;
        for(int i = 1; i < size; i++){
            tail = new TLBEntry;
            cur->next=tail;
            tail->previous = cur;
            tail->next = NULL;
            cur = cur->next;
        }

    }
    /*
    * Append a new TLB Entry into the end of the LRU Linked List
    * Will only be used in the LRU Implementation
    */
    void append(TLBEntry *t)
    {
        //write your code here
        tail->next = t;
        t->previous= tail;
        t->next=NULL;
        tail = t;
    }
    /*
    * Detach a TLB Entry from the LRU Linked List
    * Will only be used in the LRU Implementation
    */
    void detach(TLBEntry *t)
    {
        //write your code here


        TLBEntry * tempPrev = t->previous;
        TLBEntry *tempNext = t->next;

        if(tempPrev) tempPrev->next = tempNext;
        else head = tempNext;

        if(tempNext) tempNext->previous=tempPrev;
        else tail = tempPrev;

        t->previous=NULL;
        t->next=NULL;
    }


    LookupResult lookup(unsigned char pageNumber){
        //write your code here
        unsigned index = rollingHash(pageNumber)%cacheSize;
        HashTableEntry * pEntry=hTable->table+index;

        LookupResult ret;
        ret.isFound = false;
        for(list< pair <unsigned char, TLBEntry * > > :: iterator it= pEntry->hashedPointerList.begin(); it!=pEntry->hashedPointerList.end(); it++){
                if(it->first == pageNumber){
                    ret.isFound =true;
                    ret.frameIndex = it->second->physicalFrameIndex;
                    TLBEntry * temp = it->second;
                    detach(temp);
                    append(temp);
                    break;
                }
        }
        return ret;
    }

    void insert(unsigned char pageNumber, unsigned char frameIndex)
    {
        //write your code here
        TLBEntry * temp = head;
        invalidate(temp->virtualPageNo);
        detach(head);
        //
        temp->physicalFrameIndex = frameIndex;
        temp->virtualPageNo=pageNumber;
        //
        append(temp);
        //
        // cout << " ok append"<< endl;
        hTable->insert(pageNumber,temp,cacheSize);
        // cout << " ok insert"<< endl;

    }

    void print()
    {
        //write your code here
        TLBEntry * cur = tail;
        printf("-----\n");
        while(cur!=NULL){
            printf("%c   %c\n", cur->virtualPageNo, cur->physicalFrameIndex);
            cur = cur->previous;
        }
        printf("-----\n\n");
    }
    void invalidate(unsigned char pageNumber)
    {
        //write your code here
        unsigned index = rollingHash(pageNumber)%cacheSize;
        HashTableEntry * pEntry=hTable->table+index;

        for(list< pair <unsigned char, TLBEntry * > > :: iterator it= pEntry->hashedPointerList.begin(); it!=pEntry->hashedPointerList.end(); it++){
                if(it->first == pageNumber){
                	TLBEntry * temp = it->second;
                	detach(temp);
                	head->previous = temp;
                	temp->next = head;
                	head=temp;
                	temp->physicalFrameIndex = '-';
                	temp->virtualPageNo='-';

                    pEntry->hashedPointerList.erase(it);
                    break;
                }
        }

    }
};
#endif
