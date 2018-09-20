#ifndef Configuration_H
#define Configuration_H
bool debugMode = true;
unsigned int numberOfTLBEntries = 4;
unsigned int numberOfPageFrames = 256;
unsigned int numberOfInstructions = 1000;
unsigned int numberOfVirtualPages = 256;
unsigned int pageChangeAfterInstructionCount = 125;
#endif
