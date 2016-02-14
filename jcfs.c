//
// Created by jcespinoza on 2/13/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jcfs.h"

unsigned long* magicNumber = (unsigned long *) "JCFS"; //Translates to 1245922899, 01001010 01000011 01000110 01010011

void writeBlock(const char* fileName, int blocknumber, int blockSize, char *buffer) {
    FILE* file = fopen(fileName, "r+b");
    if(file != NULL){
        fseek(file, blocknumber * blockSize, SEEK_SET);
        fwrite(buffer, 1, blockSize, file);
    }
    fclose(file);
    free(file);
}

void readBlock(const char* fileName, int blocknumber, int blockSize, char *buffer) {
    FILE *file = fopen(fileName, "rb");
    if(file != NULL){
        fseek(file, blocknumber * blockSize, SEEK_SET);
        fread(buffer, 1, blockSize, file);
    }
    fclose(file);
    free(file);
}

void createDisc(const char* fileName) {
    FILE *file = fopen(fileName, "wb");
    fclose(file);
    free(file);
}

void initializeMetaBlock(int diskSize, int blockSize, MetaBlock *metaBlock){
    metaBlock->FATBlock = 2;
    metaBlock->FATSize = (diskSize / blockSize) / (blockSize / sizeof(int));
    //if(metaBlock->FATSize < 1) metaBlock->FATSize = 1;
    metaBlock->HeadBlock = metaBlock->FATBlock + metaBlock->FATSize;
}

void format(const char* fileName, int diskSize, int blockSize) {
    MetaBlock metaBlock;
    initializeMetaBlock(diskSize, blockSize, &metaBlock);
    char* buffer = (char*) malloc(blockSize);
    memcpy(buffer, &metaBlock, sizeof(metaBlock));

    DiskInfo info;
    memcpy(&(info.system), "JCFS", sizeof(char));
    info.DiskSize = diskSize;
    info.BlockSize = blockSize;

    writeBlock(fileName, 0, sizeof(DiskInfo), (char*)&info);
    writeBlock(fileName, 1, blockSize, buffer);

    int *fat = (int*)malloc(metaBlock.FATSize * blockSize);
    int i = 0;
    for(i = 0; i < metaBlock.HeadBlock; i++){
        fat[i] = -1;
    }

    for(i; i < (metaBlock.FATSize*blockSize) - 1; i++){
        fat[i] = i + 1;
    }

    fat[i] = 0;

    for(i = 0; metaBlock.FATSize; i++){
        writeBlock(fileName, metaBlock.FATBlock + i, blockSize, (char*)(fat + (i*1024)));
    }

    free(buffer);
    free(fat);
}

bool isJSFormat(DiskInfo* diskInfo){
    if(diskInfo == NULL) return false;

    unsigned long compatibilityFlag;
    memcpy(&compatibilityFlag, &(diskInfo->system), 4*sizeof(char));
    bool isCompatible = compatibilityFlag == *magicNumber;
    return isCompatible;
}

void readDiskInfo(const char *fileName, struct DiskInfo *diskInfo) {
    readBlock(fileName, 0, sizeof(DiskInfo), (char*)diskInfo);
}
