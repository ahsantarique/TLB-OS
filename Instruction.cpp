#include <cstdlib>
#include <iostream>
#include <cstdio>

using namespace std;

#ifndef Instruction_Class
#define Instruction_Class
class Instruction
{
public:
    bool lwsw; // 0 = load, 1 = sw
    unsigned char msbAddress; // upper 8 bit of the address, index1:index2
    unsigned char lsbAddress; // lower 8 bit of the address, offset
    Instruction()
    {
        int r = rand();
        if((r % 10) == 0)
        {
            lwsw = true;
        } else {
            lwsw = false;
        }
        msbAddress = rand() % 256;
        lsbAddress = rand() % 256;
    }
    Instruction(char pageNumber)
    {
        int r = rand();
        if((r % 10) == 0)
        {
            lwsw = true;
        } else {
            lwsw = false;
        }
        msbAddress = pageNumber;
        lsbAddress = rand() % 256;
    }
    void print()
    {
        if(lwsw == 0)
        {
            cout<<"Load ";
        } else {
            cout<<"Store ";
        }
        printf("%02x%02x\n",msbAddress, lsbAddress);
    }
};
#endif
