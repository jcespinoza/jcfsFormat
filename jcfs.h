//
// Created by jcespinoza on 2/10/2016.
//

#include <stdbool.h>

#ifndef HELLOWORLD_JCFS_H
#define HELLOWORLD_JCFS_H

typedef struct DiskInfo{
    char system[4];
    int DiskSize;
    int BlockSize;
} DiskInfo;

typedef struct MetaBlock{
    int FATBlock;
    int FATSize;
    int HeadBlock;
} MetaBlock;

void writeBlock(const char* fileName, int blocknumber, int blockSize, char* buffer);

void readBlock(const char* fileName, int blocknumber, int blockSize, char* buffer);

int allocateBlock(struct MetaBlock* metaBlock, const char* fileName);

int deleteBlock(struct MetaBlock* metaBlock, const char* fileName);

void createDisc(const char* diskName);

void format(const char* fileName, int diskSize, int blockSize);

bool isJSFormat(DiskInfo* diskInfo);

void readDiskInfo(const char* fileName, struct DiskInfo* diskInfo);

#endif //HELLOWORLD_JCFS_H