//
// Created by jcespinoza on 2/13/2016.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jcfs.h"

#define MAX_NAME_SIZE 256
#define DEFAULT_DISK_SIZE 1048576
#define DEFAULT_BLOCK_SIZE 1024

void printDisclaimer();
void printMainMenu();
void handleCreateDisk();
void handleMountDisk();
void handleAllocateBlock();
void handleEraseBlock();
void handleFreeSpaceInfo();
void handleInvalidOption();
void setGlobalVariables(int diskSize, int blockSize, char* filename, bool isDiskMounted);
void callMountDisk(DiskInfo info, char* fileName);
void handleUnMountDisk();

int defaultDiskSize = DEFAULT_DISK_SIZE;
int defaultBlockSize = DEFAULT_BLOCK_SIZE;
char defaultFileName[MAX_NAME_SIZE];
bool diskMounted = false;

MetaBlock currentMetaBlock;

int main()
{
    printDisclaimer();
    int answer = 0;
    do {
        printMainMenu();
        scanf("%d", &answer);

        switch(answer){
            case 1:
                handleCreateDisk();
                break;
            case 2:
                handleMountDisk();
                break;
            case 3:
                handleUnMountDisk();
                break;
            case 4:
                handleAllocateBlock();
                break;
            case 5:
                handleEraseBlock();
                break;
            case 6:
                handleFreeSpaceInfo();
                break;
            default:
                answer = 0;
                handleInvalidOption();
        }
    }while(answer != 0);
}

void printDisclaimer() {
    printf("\n\n\n[!] WARNING [!] \n\nThis program might corrupt your system if not used carefully");
    printf("\nBlank spaces in filenames not supported");
    printf("\nUse at your own risk\n\n\n");
}

void printMainMenu() {
    printf("\n\n== JCFS Format Utility ===");
    if(diskMounted) { printf("\nMounted Disk: %s", defaultFileName); }
    printf("\n    1. Create Disk / Format");
    printf("\n    2. Mount Disk");
    printf("\n    3. UnMount Disk");
    printf("\n    4. Allocate Block");
    printf("\n    5. Erase Block");
    printf("\n    6. Free Space Info");
    printf("\n\n   > ");
}

void handleCreateDisk(){
    int diskSize;
    int blockSize;
    char fileName[MAX_NAME_SIZE];
    printf("Enter the filename: ");
    scanf("%s", fileName);
    printf("Enter the disk size in bytes: ");
    scanf("%d", &diskSize);
    printf("Enter the block size in bytes: ");
    scanf("%d", &blockSize);
    printf("\nCreating disk...");
    createDisc(fileName);
    format(fileName, diskSize, blockSize);
    printf("\nDisk created successfully.");
    printf("\nMount disk [1/0]? 1 = Yes; 0 = No  ");
    int answer = 1;
    scanf("%d", &answer);
    if(answer == 1){
        setGlobalVariables(diskSize, blockSize, fileName, true);
    }
}

void handleMountDisk(){
    char fileName[MAX_NAME_SIZE];
    printf("Enter the filename: ");
    scanf("%s", fileName);
    DiskInfo info;
    readDiskInfo(fileName, &info);
    bool compatible = isJSFormat(&info);
    if(compatible){
        printf("\nMount disk %s [1=Yes/0=No]? Choice: ", fileName);
        int mount;
        scanf("%d", &mount);
        if(mount == 1) callMountDisk(info, fileName);
    }else{
        printf("\nDisk not compatible");
    }
}

void handleUnMountDisk(){
    setGlobalVariables(DEFAULT_DISK_SIZE, DEFAULT_BLOCK_SIZE, "./disk.dsk", false);
    printf("\n Disk Unmounted");
}

void setGlobalVariables(int diskSize, int blockSize, char* filename, bool hasMountedDisk){
    memcpy(defaultFileName, filename, MAX_NAME_SIZE);
    defaultDiskSize = diskSize;
    defaultBlockSize = blockSize;
    diskMounted = hasMountedDisk;
}

void callMountDisk(DiskInfo info, char* fileName){
    setGlobalVariables(info.DiskSize, info.BlockSize, fileName, true);
    printf("\nDisk Mounted");
}

void handleAllocateBlock(){

}

void handleEraseBlock(){

}

void handleFreeSpaceInfo(){

}

void handleInvalidOption(){
    printf("Invalid option. Exiting...");
}
